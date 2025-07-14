/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-07-31
* | Info        :   
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of theex Software, and to permit persons to  whom the Software is
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
#include "DEV_Config.h"


/**
 * GPIO read and write
**/
void DEV_Digital_Write(uint16_t Pin, uint8_t Value) {
	gpio_put(Pin, Value);
}

uint8_t DEV_Digital_Read(uint16_t Pin) {
	return gpio_get(Pin);
}

/**
 * SPI
**/
void DEV_SPI_WriteByte(uint8_t Value) {
	spi_write_blocking(EPD_SPI_PORT, &Value, 1);
}

void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len) {
	spi_write_blocking(EPD_SPI_PORT, pData, Len);
}

/**
 * @brief I2C write byte
 * 
 * @param Reg 
 * @param Value 
 */
extern "C" void I2C_Write_Byte(uint8_t Reg, uint8_t Value) {
	uint8_t wbuff[2] = { Reg, Value };
	i2c_write_blocking(RTC_I2C_PORT, RTC_I2C_Address, wbuff, 2, false);
}

/**
 * @brief I2C read byte
 * 
 * @param Reg 
 * @return uint8_t 
 */
extern "C" uint8_t I2C_Read_Byte(uint8_t Reg) {
	uint8_t Value;
	i2c_write_blocking(RTC_I2C_PORT, RTC_I2C_Address, &Reg, 1, false);
	i2c_read_blocking(RTC_I2C_PORT, RTC_I2C_Address, &Value, 1, false);
	return Value;
}

/******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
Info:
******************************************************************************/
void DEV_Module_Exit(void) {
	printf("[%s]: Module exit\n", __FILE_NAME__);
}
