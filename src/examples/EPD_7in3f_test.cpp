/*****************************************************************************
* | File      	:   EPD_7in3f_test.c
* | Author      :   Waveshare team
* | Function    :   7.3inch e-Paper (F) Demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-03-13
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
#include "./EPD_Test.h"
#include "./ImageData.h"
#include "./run_File.h"
#include "../e-Paper/EPD_7in3f.h"
#include "../GUI/GUI_Paint.h"
#include "../GUI/GUI_BMPfile.h"

#include <stdlib.h>  // malloc() free()
#include <string.h>

int EPD_7in3f_display_BMP(const char *path, float vol) {
  printf("[%s]: e-Paper Init and Clear...\n", __FILE_NAME__);
  EPD_7IN3F_Init();

  //Create a new image cache
  uint8_t *BlackImage;
  uint32_t Imagesize = ((EPD_7IN3F_WIDTH % 2 == 0) ? (EPD_7IN3F_WIDTH / 2) : (EPD_7IN3F_WIDTH / 2 + 1)) * EPD_7IN3F_HEIGHT;
  if ((BlackImage = (uint8_t *)malloc(Imagesize)) == NULL) {
    printf("[%s]: Failed to apply for black memory...\n", __FILE_NAME__);
    return -1;
  }
  printf("[%s]: Paint_NewImage\n", __FILE_NAME__);
  Paint_NewImage(BlackImage, EPD_7IN3F_WIDTH, EPD_7IN3F_HEIGHT, 0, EPD_7IN3F_WHITE);
  Paint_SetScale(7);

#if 1
  run_mount();

  printf("[%s]: Display BMP: %s\n", __FILE_NAME__, path);
  Paint_SelectImage(BlackImage);
  Paint_Clear(EPD_7IN3F_WHITE);

  if (!GUI_ReadBmp_RGB_7Color(path, 0, 0)) {
    Paint_DrawString_EN(10, 42, "Image cannot be displayed.", &Font16, EPD_7IN3F_BLACK, EPD_7IN3F_WHITE);
    Paint_DrawString_EN(10, 58, path, &Font16, EPD_7IN3F_BLACK, EPD_7IN3F_WHITE);
  }

  Paint_SetRotate((Paint_GetRotate() + 180) % 360);

  if (vol < 3.3) {
    char strvol[21] = { 0 };
    sprintf(strvol, "%f V", vol);

    Paint_DrawString_EN(10, 10, "Low voltage, please charge in time.", &Font16, EPD_7IN3F_BLACK, EPD_7IN3F_WHITE);
    Paint_DrawString_EN(10, 26, strvol, &Font16, EPD_7IN3F_BLACK, EPD_7IN3F_WHITE);
  }

  printf("[%s]: EPD_Display\n", __FILE_NAME__);
  EPD_7IN3F_Display(BlackImage);

  run_unmount();
#endif
  printf("[%s]: Update Path Index...\n", __FILE_NAME__);
  updatePathIndex();

  printf("[%s]: Goto Sleep...\n", __FILE_NAME__);
  EPD_7IN3F_Sleep();
  free(BlackImage);
  BlackImage = NULL;

  return 0;
}

int EPD_7in3f_display(float vol) {
  printf("[%s]: e-Paper Init and Clear...\n", __FILE_NAME__);
  EPD_7IN3F_Init();

  //Create a new image cache
  uint8_t *BlackImage;
  uint32_t Imagesize = ((EPD_7IN3F_WIDTH % 2 == 0) ? (EPD_7IN3F_WIDTH / 2) : (EPD_7IN3F_WIDTH / 2 + 1)) * EPD_7IN3F_HEIGHT;
  if ((BlackImage = (uint8_t *)malloc(Imagesize)) == NULL) {
    printf("[%s]: Failed to apply for black memory...\n", __FILE_NAME__);
    return -1;
  }
  printf("[%s]: Paint_NewImage\n", __FILE_NAME__);
  Paint_NewImage(BlackImage, EPD_7IN3F_WIDTH, EPD_7IN3F_HEIGHT, 0, EPD_7IN3F_WHITE);
  Paint_SetScale(7);

  printf("[%s]: Display BMP\n", __FILE_NAME__);
  Paint_SelectImage(BlackImage);
  Paint_Clear(EPD_7IN3F_WHITE);

  Paint_DrawBitMap(Image7color);

  Paint_SetRotate(270);
  char strvol[21] = { 0 };
  sprintf(strvol, "%f V", vol);
  if (vol < 3.3) {
    Paint_DrawString_EN(10, 10, "Low voltage, please charge in time.", &Font16, EPD_7IN3F_BLACK, EPD_7IN3F_WHITE);
    Paint_DrawString_EN(10, 26, strvol, &Font16, EPD_7IN3F_BLACK, EPD_7IN3F_WHITE);
  }

  printf("[%s]: EPD_Display\n", __FILE_NAME__);
  EPD_7IN3F_Display(BlackImage);

  printf("[%s]: Goto Sleep...\n", __FILE_NAME__);
  EPD_7IN3F_Sleep();
  free(BlackImage);
  BlackImage = NULL;

  return 0;
}

int EPD_7in3f_test(void) {
  printf("[%s]: e-Paper Init and Clear...\n", __FILE_NAME__);
  EPD_7IN3F_Init();

  //Create a new image cache
  uint8_t *BlackImage;
  uint32_t Imagesize = ((EPD_7IN3F_WIDTH % 2 == 0) ? (EPD_7IN3F_WIDTH / 2) : (EPD_7IN3F_WIDTH / 2 + 1)) * EPD_7IN3F_HEIGHT;
  if ((BlackImage = (uint8_t *)malloc(Imagesize)) == NULL) {
    printf("[%s]: Failed to apply for black memory...\n", __FILE_NAME__);
    return -1;
  }
  printf("[%s]: Paint_NewImage\n", __FILE_NAME__);
  Paint_NewImage(BlackImage, EPD_7IN3F_WIDTH, EPD_7IN3F_HEIGHT, 0, EPD_7IN3F_WHITE);
  Paint_SetScale(7);

#if 1  // Drawing on the image
  //1.Select Image
  printf("[%s]: SelectImage:BlackImage\n", __FILE_NAME__);
  Paint_SelectImage(BlackImage);
  Paint_Clear(EPD_7IN3F_WHITE);

  int hNumber, hWidth, vNumber, vWidth;
  hNumber = 20;
  hWidth = EPD_7IN3F_HEIGHT / hNumber;  // 800/20
  vNumber = 10;
  vWidth = EPD_7IN3F_WIDTH / vNumber;  // 480/10

  // 2.Drawing on the image
  printf("[%s]: Drawing:BlackImage\n", __FILE_NAME__);
  for (int i = 0; i < vNumber; i++) {
    Paint_DrawRectangle(1, 1 + i * vWidth, 800, vWidth * (i + 1), EPD_7IN3F_GREEN + (i % 5), DOT_PIXEL_1X1, DRAW_FILL_FULL);
  }
  for (int i = 0, j = 0; i < hNumber; i++) {
    if (i % 2) {
      j++;
      Paint_DrawRectangle(1 + i * hWidth, 1, hWidth * (1 + i), 480, j % 2 ? EPD_7IN3F_BLACK : EPD_7IN3F_WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    }
  }

  printf("[%s]: EPD_Display\n", __FILE_NAME__);
  EPD_7IN3F_Display(BlackImage);
#endif

  printf("[%s]: Goto Sleep...\n", __FILE_NAME__);
  EPD_7IN3F_Sleep();
  free(BlackImage);
  BlackImage = NULL;

  return 0;
}
