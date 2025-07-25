/*****************************************************************************
* | File      	:   EPD_7in3f.c
* | Author      :   Waveshare team
* | Function    :   7.3inch e-Paper (F) Driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-08-04
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_7in3f.h"
#include <Arduino.h>

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_7IN3F_Reset(void) {
  gpio_put(EPD_RST_PIN, 1);
  delay(20);
  gpio_put(EPD_RST_PIN, 0);
  delay(5);
  gpio_put(EPD_RST_PIN, 1);
  delay(20);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_7IN3F_SendCommand(uint8_t Reg) {
  gpio_put(EPD_DC_PIN, 0);
  gpio_put(EPD_CS_PIN, 0);
  DEV_SPI_WriteByte(Reg);
  gpio_put(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_7IN3F_SendData(uint8_t Data) {
  gpio_put(EPD_DC_PIN, 1);
  gpio_put(EPD_CS_PIN, 0);
  DEV_SPI_WriteByte(Data);
  gpio_put(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_7IN3F_ReadBusyH(void) {
  int cnt = 0;
  watchdog_update();
  printf("[%s]: e-Paper busy H\n", __FILE_NAME__);
  while (!gpio_get(EPD_BUSY_PIN)) {  //LOW: idle, HIGH: busy
    delay(10);
    cnt++;
    if (cnt > 5000) {
      printf("[%s]: e-Paper busy H force release\n", __FILE_NAME__);
      return;
    }
  }
  printf("[%s]: e-Paper busy H release\n", __FILE_NAME__);
}
static void EPD_7IN3F_ReadBusyL(void) {
  watchdog_update();
  printf("[%s]: e-Paper busy L\n", __FILE_NAME__);
  while (gpio_get(EPD_BUSY_PIN)) {  //LOW: idle, HIGH: busy
    delay(5);
  }
  printf("[%s]: e-Paper busy L release\n", __FILE_NAME__);
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_7IN3F_TurnOnDisplay(void) {
  EPD_7IN3F_SendCommand(0x04);  // POWER_ON
  EPD_7IN3F_ReadBusyH();

  EPD_7IN3F_SendCommand(0x12);  // DISPLAY_REFRESH
  EPD_7IN3F_SendData(0x00);
  EPD_7IN3F_ReadBusyH();

  EPD_7IN3F_SendCommand(0x02);  // POWER_OFF
  EPD_7IN3F_SendData(0X00);
  EPD_7IN3F_ReadBusyH();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_7IN3F_Init(void) {
  EPD_7IN3F_Reset();
  EPD_7IN3F_ReadBusyH();
  delay(30);

  EPD_7IN3F_SendCommand(0xAA);  // CMDH
  EPD_7IN3F_SendData(0x49);
  EPD_7IN3F_SendData(0x55);
  EPD_7IN3F_SendData(0x20);
  EPD_7IN3F_SendData(0x08);
  EPD_7IN3F_SendData(0x09);
  EPD_7IN3F_SendData(0x18);

  EPD_7IN3F_SendCommand(0x01);
  EPD_7IN3F_SendData(0x3F);
  EPD_7IN3F_SendData(0x00);
  EPD_7IN3F_SendData(0x32);
  EPD_7IN3F_SendData(0x2A);
  EPD_7IN3F_SendData(0x0E);
  EPD_7IN3F_SendData(0x2A);

  EPD_7IN3F_SendCommand(0x00);
  EPD_7IN3F_SendData(0x5F);
  EPD_7IN3F_SendData(0x69);

  EPD_7IN3F_SendCommand(0x03);
  EPD_7IN3F_SendData(0x00);
  EPD_7IN3F_SendData(0x54);
  EPD_7IN3F_SendData(0x00);
  EPD_7IN3F_SendData(0x44);

  EPD_7IN3F_SendCommand(0x05);
  EPD_7IN3F_SendData(0x40);
  EPD_7IN3F_SendData(0x1F);
  EPD_7IN3F_SendData(0x1F);
  EPD_7IN3F_SendData(0x2C);

  EPD_7IN3F_SendCommand(0x06);
  EPD_7IN3F_SendData(0x6F);
  EPD_7IN3F_SendData(0x1F);
  EPD_7IN3F_SendData(0x1F);
  EPD_7IN3F_SendData(0x22);

  EPD_7IN3F_SendCommand(0x08);
  EPD_7IN3F_SendData(0x6F);
  EPD_7IN3F_SendData(0x1F);
  EPD_7IN3F_SendData(0x1F);
  EPD_7IN3F_SendData(0x22);

  EPD_7IN3F_SendCommand(0x13);  // IPC
  EPD_7IN3F_SendData(0x00);
  EPD_7IN3F_SendData(0x04);

  EPD_7IN3F_SendCommand(0x30);
  EPD_7IN3F_SendData(0x3C);

  EPD_7IN3F_SendCommand(0x41);  // TSE
  EPD_7IN3F_SendData(0x00);

  EPD_7IN3F_SendCommand(0x50);
  EPD_7IN3F_SendData(0x3F);

  EPD_7IN3F_SendCommand(0x60);
  EPD_7IN3F_SendData(0x02);
  EPD_7IN3F_SendData(0x00);

  EPD_7IN3F_SendCommand(0x61);
  EPD_7IN3F_SendData(0x03);
  EPD_7IN3F_SendData(0x20);
  EPD_7IN3F_SendData(0x01);
  EPD_7IN3F_SendData(0xE0);

  EPD_7IN3F_SendCommand(0x82);
  EPD_7IN3F_SendData(0x1E);

  EPD_7IN3F_SendCommand(0x84);
  EPD_7IN3F_SendData(0x00);

  EPD_7IN3F_SendCommand(0x86);  // AGID
  EPD_7IN3F_SendData(0x00);

  EPD_7IN3F_SendCommand(0xE3);
  EPD_7IN3F_SendData(0x2F);

  EPD_7IN3F_SendCommand(0xE0);  // CCSET
  EPD_7IN3F_SendData(0x00);

  EPD_7IN3F_SendCommand(0xE6);  // TSSET
  EPD_7IN3F_SendData(0x00);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_7IN3F_Clear(uint8_t color) {
  uint16_t Width, Height;
  Width = (EPD_7IN3F_WIDTH % 2 == 0) ? (EPD_7IN3F_WIDTH / 2) : (EPD_7IN3F_WIDTH / 2 + 1);
  Height = EPD_7IN3F_HEIGHT;

  EPD_7IN3F_SendCommand(0x10);
  for (uint16_t j = 0; j < Height; j++) {
    for (uint16_t i = 0; i < Width; i++) {
      EPD_7IN3F_SendData((color << 4) | color);
    }
  }

  EPD_7IN3F_TurnOnDisplay();
}

/******************************************************************************
function :	show 7 kind of color block
parameter:
******************************************************************************/
void EPD_7IN3F_Show7Block(void) {
  unsigned long i, j, k;
  unsigned char const Color_seven[8] = { EPD_7IN3F_BLACK, EPD_7IN3F_BLUE, EPD_7IN3F_GREEN, EPD_7IN3F_ORANGE,
                                         EPD_7IN3F_RED, EPD_7IN3F_YELLOW, EPD_7IN3F_WHITE, EPD_7IN3F_WHITE };

  EPD_7IN3F_SendCommand(0x10);
  for (i = 0; i < 240; i++) {
    for (k = 0; k < 4; k++) {
      for (j = 0; j < 100; j++) {
        EPD_7IN3F_SendData((Color_seven[k] << 4) | Color_seven[k]);
      }
    }
  }
  for (i = 0; i < 240; i++) {
    for (k = 4; k < 8; k++) {
      for (j = 0; j < 100; j++) {
        EPD_7IN3F_SendData((Color_seven[k] << 4) | Color_seven[k]);
      }
    }
  }
  EPD_7IN3F_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_7IN3F_Display(uint8_t *Image) {
  uint16_t Width, Height;
  Width = (EPD_7IN3F_WIDTH % 2 == 0) ? (EPD_7IN3F_WIDTH / 2) : (EPD_7IN3F_WIDTH / 2 + 1);
  Height = EPD_7IN3F_HEIGHT;

  EPD_7IN3F_SendCommand(0x10);
  for (uint16_t j = 0; j < Height; j++) {
    for (uint16_t i = 0; i < Width; i++) {
      EPD_7IN3F_SendData(Image[i + j * Width]);
    }
  }
  EPD_7IN3F_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_7IN3F_Sleep(void) {
  EPD_7IN3F_SendCommand(0x07);  // DEEP_SLEEP
  EPD_7IN3F_SendData(0XA5);
}
