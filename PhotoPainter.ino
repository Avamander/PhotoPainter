#include "fatfs-spi.h"
#include <SPI.h>
#include "src/examples/EPD_Test.h"  // Examples
#include "src/Config/DEV_Config.h"

#include "src/examples/run_File.h"
extern "C" {
#include "src/RTC/waveshare_PCF85063.h"  // RT
}

#include <time.h>

extern const char *fileList;
extern char pathName[];


#define enChargingRtc 0

/*
Mode 0: Automatically get pic folder names and sort them
Mode 1: Automatically get pic folder names but not sorted
Mode 2: pic folder name is not automatically obtained, users need to create fileList.txt file and write the picture name in TF card by themselves
*/
#define Mode 0


float measureVBAT() {
  float voltage = 0.0;
  const float conversion_factor = 3.3f / (1 << 12);
  uint16_t result = adc_read();
  voltage = result * conversion_factor * 3;
  printf("[%s]: Raw value: 0x%03x, voltage: %f V\n", __FILE_NAME__, result, voltage);
  return voltage;
}

void chargeState_callback() {
  if (gpio_get(VBUS)) {
    if (!gpio_get(CHARGE_STATE)) {  // is charging
      ledCharging();
    } else {  // charge complete
      ledCharged();
    }
  }
}

void run_display(Time_data Time, Time_data alarmTime, char hasCard) {
  if (hasCard) {
    setFilePath();
    EPD_7in3f_display_BMP(pathName, measureVBAT());  // display bmp
  } else {
    EPD_7in3f_display(measureVBAT());
  }

  PCF85063_clear_alarm_flag();   // clear RTC alarm flag
  rtcRunAlarm(Time, alarmTime);  // RTC run alarm
}

void setup() {
  Serial.begin(115200);

  Time_data Time = { 2025 - 2000, 3, 31, 0, 0, 0 };
  Time_data alarmTime = Time;
  // alarmTime.seconds += 10;
  // alarmTime.minutes += 30;
  alarmTime.hours += 24;
  char isCard = 0;

  printf("[%s]: Init...\n", __FILE_NAME__);
  if (DEV_Module_Init() != 0) {  // DEV init
    //return -1;
  }

  //watchdog_enable(30 * 1000, 1);  // 8s
  delay(1000);
  PCF85063_init();               // RTC init
  rtcRunAlarm(Time, alarmTime);  // RTC run alarm
  gpio_set_irq_enabled_with_callback(CHARGE_STATE, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, reinterpret_cast<gpio_irq_callback_t>(chargeState_callback));

  if (measureVBAT() < 3.1) {  // battery power is low
    printf("[%s]: low power...\n", __FILE_NAME__);
    PCF85063_alarm_Time_Disable();
    ledLowPower();  // LED flash for Low power
    powerOff();     // BAT off
    //return 0;
  } else {
    printf("[%s]: work...\n", __FILE_NAME__);
    ledPowerOn();
  }

  if (!sdTest()) {
    isCard = 1;
    if (Mode == 0) {
      sdScanDir();
      file_sort();
    }
    if (Mode == 1) {
      sdScanDir();
    }
    if (Mode == 2) {
      file_cat();
    }

  } else {
    isCard = 0;
  }

  if (!gpio_get(VBUS)) {  // no charge state
    run_display(Time, alarmTime, isCard);
  } else {  // charge state
    chargeState_callback();
    while (gpio_get(VBUS)) {
      measureVBAT();

#if enChargingRtc
      if (!gpio_get(RTC_INT)) {  // RTC interrupt trigger
        printf("[%s]: rtc interrupt\n", __FILE_NAME__);
        watchdog_update();
        run_display(Time, alarmTime, isCard);
      }
#endif

      if (!gpio_get(BAT_STATE)) {  // KEY pressed
        printf("[%s]: key interrupt\n", __FILE_NAME__);
        watchdog_update();
        run_display(Time, alarmTime, isCard);
      }
      delay(200);
    }
  }


  printf("[%s]: power off...\n", __FILE_NAME__);
  powerOff();  // BAT off

  //return 0;
}

void ledPowerOn(void) {
  for (int i = 0; i < 3; i++) {
    gpio_put(LED_ACT, 1);
    delay(200);
    gpio_put(LED_ACT, 0);
    delay(100);
  }
}

void ledLowPower(void) {
  for (int i = 0; i < 5; i++) {
    gpio_put(LED_PWR, 1);
    delay(200);
    gpio_put(LED_PWR, 0);
    delay(100);
  }
}

void ledCharging(void) {
  gpio_put(LED_PWR, 1);
}

void ledCharged(void) {
  gpio_put(LED_PWR, 0);
}

void powerOff(void) {
  gpio_put(BAT_OFF, 0);
}

uint8_t DEV_Module_Init() {
  spi_init(EPD_SPI_PORT, 8000 * 1000);
  gpio_set_function(EPD_CLK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(EPD_MOSI_PIN, GPIO_FUNC_SPI);

  spi_init(SD_SPI_PORT, 12500 * 1000);
  gpio_set_function(SD_CLK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(SD_MOSI_PIN, GPIO_FUNC_SPI);
  gpio_set_function(SD_MISO_PIN, GPIO_FUNC_SPI);

  i2c_init(RTC_I2C_PORT, 100 * 1000);
  gpio_set_function(RTC_SDA, GPIO_FUNC_I2C);
  gpio_set_function(RTC_SCL, GPIO_FUNC_I2C);

  adc_init();
  adc_gpio_init(VBAT);
  adc_select_input(3);

  // GPIO Config
  DEV_GPIO_Init();

  printf("[%s]: DEV_Module_Init OK\n", __FILE_NAME__);
  return 0;
}

void DEV_GPIO_Init(void) {
  // EPD
  gpio_init(EPD_RST_PIN);
  gpio_set_dir(EPD_RST_PIN, GPIO_OUT);
  gpio_init(EPD_DC_PIN);
  gpio_set_dir(EPD_DC_PIN, GPIO_OUT);
  gpio_init(EPD_CS_PIN);
  gpio_set_dir(EPD_CS_PIN, GPIO_OUT);
  gpio_init(EPD_BUSY_PIN);
  gpio_set_dir(EPD_BUSY_PIN, GPIO_IN);
  // LED
  gpio_init(LED_ACT);
  gpio_set_dir(LED_ACT, GPIO_OUT);
  gpio_init(LED_PWR);
  gpio_set_dir(LED_PWR, GPIO_OUT);
  // SDCARD
  gpio_init(SD_CS_PIN);
  gpio_set_dir(SD_CS_PIN, GPIO_OUT);
  // RTC
  gpio_init(RTC_INT);
  gpio_set_dir(RTC_INT, GPIO_IN);
  gpio_pull_up(RTC_INT);
  // BAT
  gpio_init(BAT_OFF);
  gpio_set_dir(BAT_OFF, GPIO_OUT);
  gpio_init(BAT_STATE);
  gpio_set_dir(BAT_STATE, GPIO_IN);
  gpio_pull_up(BAT_STATE);
  gpio_init(CHARGE_STATE);
  gpio_set_dir(CHARGE_STATE, GPIO_IN);
  gpio_pull_up(CHARGE_STATE);
  // POWER
  gpio_init(EPD_POWER_EN);
  gpio_set_dir(EPD_POWER_EN, GPIO_OUT);
  gpio_init(VBUS);
  gpio_set_dir(VBUS, GPIO_IN);

  gpio_put(LED_ACT, 0);  // LED off
  gpio_put(LED_PWR, 0);  // LED off
  gpio_put(EPD_CS_PIN, 1);
  gpio_put(BAT_OFF, 1);       // BAT on
  gpio_put(EPD_POWER_EN, 1);  // EPD power on
}

void loop() {
}