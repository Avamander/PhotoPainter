/*****************************************************************************
* | File      	:   GUI_BMPfile.h
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                   Used to shield the underlying layers of each master
*                   and enhance portability
*----------------
* |	This version:   V1.0
* | Date        :   2022-10-11
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
#include "GUI_BMPfile.h"
#include "GUI_Paint.h"
#include "DEV_Config.h"
#include "Debug.h"

#include <stdlib.h>	//exit()
#include <string.h> //memset()
#include <math.h> //memset()

#include "f_util.h"
#include "ff.h"

#define GUI_COLORMAP_NUM_COLORS 7
const uint8_t GUI_ColorMap[GUI_COLORMAP_NUM_COLORS][3] = {
        {0, 0, 0}, // Black
        {255, 255, 255}, // White
        {0, 255, 0}, // Green
        {0, 0, 255}, // Blue
        {255, 0, 0}, // Red
        {255, 255, 0}, // Yellow
        {255, 127, 0}, // Orange
};

const uint8_t GUI_GammaLut[256] = {
        /*^1.8 benedikt*///0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 21, 21, 22, 22, 23, 24, 24, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 38, 38, 39, 40, 41, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 86, 87, 88, 89, 90, 91, 92, 93, 95, 96, 97, 98, 99, 100, 102, 103, 104, 105, 107, 108, 109, 110, 111, 113, 114, 115, 116, 118, 119, 120, 122, 123, 124, 126, 127, 128, 129, 131, 132, 134, 135, 136, 138, 139, 140, 142, 143, 145, 146, 147, 149, 150, 152, 153, 154, 156, 157, 159, 160, 162, 163, 165, 166, 168, 169, 171, 172, 174, 175, 177, 178, 180, 181, 183, 184, 186, 188, 189, 191, 192, 194, 195, 197, 199, 200, 202, 204, 205, 207, 208, 210, 212, 213, 215, 217, 218, 220, 222, 224, 225, 227, 229, 230, 232, 234, 236, 237, 239, 241, 243, 244, 246, 248, 250, 251, 253, 255
        /*^1.7 more contrast, darker*/0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 17, 17, 18, 18, 19, 19, 20, 21, 21, 22, 22, 23, 24, 24, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32, 33, 33, 34, 35, 36, 36, 37, 38, 39, 39, 40, 41, 42, 43, 43, 44, 45, 46, 47, 48, 48, 49, 50, 51, 52, 53, 54, 55, 56, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 88, 89, 90, 91, 92, 93, 94, 95, 97, 98, 99, 100, 101, 102, 103, 105, 106, 107, 108, 109, 111, 112, 113, 114, 115, 117, 118, 119, 120, 122, 123, 124, 125, 127, 128, 129, 131, 132, 133, 134, 136, 137, 138, 140, 141, 142, 144, 145, 146, 148, 149, 151, 152, 153, 155, 156, 157, 159, 160, 162, 163, 164, 166, 167, 169, 170, 172, 173, 174, 176, 177, 179, 180, 182, 183, 185, 186, 188, 189, 191, 192, 194, 195, 197, 198, 200, 201, 203, 205, 206, 208, 209, 211, 212, 214, 216, 217, 219, 220, 222, 224, 225, 227, 228, 230, 232, 233, 235, 237, 238, 240, 242, 243, 245, 247, 248, 250, 252, 253, 255
        /*^1.6 lighter, less contrast*///0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 18, 18, 19, 19, 20, 21, 21, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28, 29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 97, 98, 99, 100, 101, 102, 103, 104, 106, 107, 108, 109, 110, 111, 113, 114, 115, 116, 117, 119, 120, 121, 122, 123, 125, 126, 127, 128, 130, 131, 132, 133, 135, 136, 137, 138, 140, 141, 142, 143, 145, 146, 147, 149, 150, 151, 153, 154, 155, 157, 158, 159, 161, 162, 163, 165, 166, 167, 169, 170, 171, 173, 174, 176, 177, 178, 180, 181, 183, 184, 185, 187, 188, 190, 191, 193, 194, 196, 197, 198, 200, 201, 203, 204, 206, 207, 209, 210, 212, 213, 215, 216, 218, 219, 221, 222, 224, 225, 227, 228, 230, 231, 233, 235, 236, 238, 239, 241, 242, 244, 245, 247, 249, 250, 252, 253, 255
        /*^1.5*///0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 14, 14, 15, 15, 16, 16, 17, 18, 18, 19, 20, 20, 21, 21, 22, 23, 23, 24, 25, 26, 26, 27, 28, 28, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40, 41, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 102, 103, 104, 105, 106, 107, 108, 109, 110, 112, 113, 114, 115, 116, 117, 119, 120, 121, 122, 123, 124, 126, 127, 128, 129, 130, 132, 133, 134, 135, 136, 138, 139, 140, 141, 142, 144, 145, 146, 147, 149, 150, 151, 152, 154, 155, 156, 158, 159, 160, 161, 163, 164, 165, 167, 168, 169, 171, 172, 173, 174, 176, 177, 178, 180, 181, 182, 184, 185, 187, 188, 189, 191, 192, 193, 195, 196, 197, 199, 200, 202, 203, 204, 206, 207, 209, 210, 211, 213, 214, 216, 217, 218, 220, 221, 223, 224, 226, 227, 228, 230, 231, 233, 234, 236, 237, 239, 240, 242, 243, 245, 246, 248, 249, 251, 252, 254, 255
        /*^1.4*///0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 12, 12, 13, 13, 14, 15, 15, 16, 16, 17, 18, 18, 19, 20, 20, 21, 22, 22, 23, 24, 25, 25, 26, 27, 28, 28, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42, 43, 43, 44, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 54, 55, 56, 57, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 115, 116, 117, 118, 119, 120, 121, 122, 124, 125, 126, 127, 128, 129, 130, 132, 133, 134, 135, 136, 137, 139, 140, 141, 142, 143, 145, 146, 147, 148, 149, 151, 152, 153, 154, 155, 157, 158, 159, 160, 161, 163, 164, 165, 166, 168, 169, 170, 171, 173, 174, 175, 176, 178, 179, 180, 181, 183, 184, 185, 187, 188, 189, 190, 192, 193, 194, 196, 197, 198, 200, 201, 202, 203, 205, 206, 207, 209, 210, 211, 213, 214, 215, 217, 218, 219, 221, 222, 223, 225, 226, 227, 229, 230, 232, 233, 234, 236, 237, 238, 240, 241, 242, 244, 245, 247, 248, 249, 251, 252, 254, 255
        /*^0.7*///0, 5, 9, 11, 14, 16, 18, 21, 23, 25, 26, 28, 30, 32, 33, 35, 37, 38, 40, 41, 43, 44, 46, 47, 49, 50, 52, 53, 54, 56, 57, 58, 60, 61, 62, 64, 65, 66, 67, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 82, 83, 84, 85, 86, 87, 88, 89, 90, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 142, 143, 144, 145, 146, 147, 148, 149, 150, 150, 151, 152, 153, 154, 155, 156, 157, 157, 158, 159, 160, 161, 162, 163, 163, 164, 165, 166, 167, 168, 168, 169, 170, 171, 172, 173, 173, 174, 175, 176, 177, 178, 178, 179, 180, 181, 182, 182, 183, 184, 185, 186, 186, 187, 188, 189, 190, 190, 191, 192, 193, 194, 194, 195, 196, 197, 197, 198, 199, 200, 201, 201, 202, 203, 204, 204, 205, 206, 207, 208, 208, 209, 210, 211, 211, 212, 213, 214, 214, 215, 216, 217, 217, 218, 219, 220, 220, 221, 222, 223, 223, 224, 225, 226, 226, 227, 228, 228, 229, 230, 231, 231, 232, 233, 234, 234, 235, 236, 237, 237, 238, 239, 239, 240, 241, 242, 242, 243, 244, 244, 245, 246, 247, 247, 248, 249, 249, 250, 251, 251, 252, 253, 254, 254, 255
        /*^0.5*///0, 16, 23, 28, 32, 36, 39, 42, 45, 48, 50, 53, 55, 58, 60, 62, 64, 66, 68, 70, 71, 73, 75, 77, 78, 80, 81, 83, 84, 86, 87, 89, 90, 92, 93, 94, 96, 97, 98, 100, 101, 102, 103, 105, 106, 107, 108, 109, 111, 112, 113, 114, 115, 116, 117, 118, 119, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 145, 146, 147, 148, 149, 150, 151, 151, 152, 153, 154, 155, 156, 156, 157, 158, 159, 160, 160, 161, 162, 163, 164, 164, 165, 166, 167, 167, 168, 169, 170, 170, 171, 172, 173, 173, 174, 175, 176, 176, 177, 178, 179, 179, 180, 181, 181, 182, 183, 183, 184, 185, 186, 186, 187, 188, 188, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195, 196, 196, 197, 198, 198, 199, 199, 200, 201, 201, 202, 203, 203, 204, 204, 205, 206, 206, 207, 208, 208, 209, 209, 210, 211, 211, 212, 212, 213, 214, 214, 215, 215, 216, 217, 217, 218, 218, 219, 220, 220, 221, 221, 222, 222, 223, 224, 224, 225, 225, 226, 226, 227, 228, 228, 229, 229, 230, 230, 231, 231, 232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238, 239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255
        /*sym sqrt felix*///0, 11, 16, 20, 23, 26, 28, 30, 32, 34, 35, 38, 39, 41, 43, 44, 45, 47, 48, 50, 50, 52, 53, 55, 55, 57, 57, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 69, 71, 72, 72, 73, 74, 75, 76, 77, 77, 79, 79, 80, 81, 81, 82, 83, 84, 84, 86, 86, 87, 88, 89, 89, 90, 91, 91, 92, 93, 94, 94, 95, 96, 96, 96, 97, 98, 98, 99, 100, 101, 101, 102, 103, 103, 103, 104, 105, 106, 106, 107, 107, 108, 108, 109, 110, 111, 111, 111, 112, 113, 113, 113, 114, 115, 115, 116, 116, 117, 118, 118, 118, 119, 120, 120, 120, 121, 122, 123, 123, 123, 124, 125, 125, 125, 126, 127, 127, 128, 128, 128, 128, 129, 130, 130, 130, 131, 132, 132, 132, 133, 134, 135, 135, 135, 136, 137, 137, 137, 138, 139, 139, 140, 140, 141, 142, 142, 142, 143, 144, 145, 145, 145, 146, 147, 147, 148, 148, 149, 149, 150, 151, 152, 152, 152, 153, 154, 154, 155, 156, 157, 157, 158, 159, 159, 159, 160, 161, 162, 162, 163, 164, 164, 165, 166, 166, 167, 168, 169, 169, 171, 171, 172, 173, 174, 174, 175, 176, 176, 178, 179, 179, 180, 181, 182, 183, 183, 184, 186, 186, 187, 188, 189, 190, 191, 192, 193, 194, 196, 196, 198, 198, 200, 200, 202, 203, 205, 205, 207, 208, 210, 211, 213, 214, 216, 217, 220, 221, 223, 225, 227, 230, 232, 235, 239, 244, 255
};

static void GUI_AddClampedDelta(uint8_t* acc, int delta) {
    int result = (int)*acc + delta;
    if (result < 0)
        *acc = 0;
    else if (result > 0xff)
        *acc = 0xff;
    else
        *acc = (uint8_t)result;
}

bool GUI_ReadBmp_RGB_7Color(const char *path, UWORD Xstart, UWORD Ystart)
{
    BMPFILEHEADER bmpFileHeader;  //Define a bmp file header structure
    BMPINFOHEADER bmpInfoHeader;  //Define a bmp info header structure
    
    FRESULT fr;
    FIL fil;
    UINT br;
    printf("open %s", path);
    fr = f_open(&fil, path, FA_READ);
    if (FR_OK != fr && FR_EXIST != fr) {
        printf("f_open(%s) error: %s (%d)\n", path, FRESULT_str(fr), fr);
        return false;
    }
    
    // Set the file pointer from the beginning
    f_lseek(&fil, 0);
    f_read(&fil, &bmpFileHeader, sizeof(BMPFILEHEADER), &br);   // sizeof(BMPFILEHEADER) must be 14
    if (br != sizeof(BMPFILEHEADER)) {
        printf("f_read bmpFileHeader error\r\n");
        // printf("br is %d\n", br);
        f_close(&fil);
        return false;
    }
    f_read(&fil, &bmpInfoHeader, sizeof(BMPINFOHEADER), &br);   // sizeof(BMPFILEHEADER) must be 50
    if (br != sizeof(BMPINFOHEADER)) {
        printf("f_read bmpInfoHeader error\r\n");
        // printf("br is %d\n", br);
        f_close(&fil);
        return false;
    }

    // handle images with negative height, where the image is mirrored horizontally
    int height = bmpInfoHeader.biHeight < 0 ? -bmpInfoHeader.biHeight : bmpInfoHeader.biHeight;
    if (bmpInfoHeader.biWidth > height)
        Paint_SetRotate(bmpInfoHeader.biHeight < 0 ? 180 : 0);
    else
        Paint_SetRotate(bmpInfoHeader.biHeight < 0 ? 270 : 90);

    printf("pixel = %d * %d\r\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    // Determine if it is a monochrome or compressed bitmap
    if (bmpInfoHeader.biBitCount != 24) {
        printf("image is not 24bpp bitmap!\r\n");
        f_close(&fil);
        return false;
    }
    if (bmpInfoHeader.biCompression != 0) {
        printf("image is compressed!\r\n");
        f_close(&fil);
        return false;
    }

    // Calculate overscan (round up to next multiple of 4)
    unsigned int overscan = ((bmpInfoHeader.biWidth*3 + 3) & ~3) - bmpInfoHeader.biWidth*3;
    printf("overscan %d\r\n", overscan);

    // Read image data into the cache
    UWORD x, y;
    UBYTE Rdata[3];
    
    f_lseek(&fil, bmpFileHeader.bOffset);

    printf("read data\n");

    // two rows of rgb888 pixel error to avoid a full image copy in memory
    // the first and last pixel columns are a border for more efficiency, as no range checks are needed
    int dither_line_width = (int)bmpInfoHeader.biWidth + 2;
    uint8_t dither_lines[dither_line_width][2][3]; // [x][y 0:1][rgb_channel 0:2]
    memset(dither_lines, 0, dither_line_width * 2 * 3);
    
    for(y = 0; y < height; y++) {//Total display column
        for(x = 0; x < bmpInfoHeader.biWidth ; x++) {//Show a line in the line
            if(f_read(&fil, Rdata, 3, &br) != FR_OK) {
                perror("get bmpdata:\r\n");
                f_close(&fil);
                return false;
            } else if (br != 3) {
                printf("early eof\r\n");
                f_close(&fil);
                return false;
            }

            // add original pixel data to dither line buffer in correct order
            for (int channel = 0; channel < 3; channel++)
                GUI_AddClampedDelta(&dither_lines[x + 1][0][channel], (int)GUI_GammaLut[Rdata[2-channel]]);

            // find closest palette color
            uint8_t color_min_delta_idx = 0;
            uint32_t color_min_delta = UINT32_MAX;
            for (int color_idx = 0; color_idx < GUI_COLORMAP_NUM_COLORS; color_idx++) {
                uint32_t color_delta = 0;
                for (int channel = 0; channel < 3; channel++) {
                    uint32_t channel_val = GUI_ColorMap[color_idx][channel]-dither_lines[x + 1][0][channel];
                    color_delta += channel_val*channel_val;
                }

                if (color_delta >= color_min_delta)
                    continue;

                color_min_delta = color_delta;
                color_min_delta_idx = color_idx;

                if (color_min_delta == 0)
                    break;
            }

            // perform dithering per color component channel
            for (int channel = 0; channel < 3; channel++) {
                int error = (int)dither_lines[x + 1][0][channel] - (int)GUI_ColorMap[color_min_delta_idx][channel];
                GUI_AddClampedDelta(&dither_lines[x + 1 + 1][0][channel], (error * 7) / 16);
                GUI_AddClampedDelta(&dither_lines[x - 1 + 1][1][channel], (error * 3) / 16);
                GUI_AddClampedDelta(&dither_lines[x + 0 + 1][1][channel], (error * 5) / 16);
                GUI_AddClampedDelta(&dither_lines[x + 1 + 1][1][channel], (error * 1) / 16);
            }

            Paint_SetPixel(Xstart + bmpInfoHeader.biWidth-1-x, Ystart + y, color_min_delta_idx);
        }

        // shift dither line buffer up
        for (int col = 0; col < dither_line_width; col++) {
            for (int channel = 0; channel < 3; channel++) {
                dither_lines[col][0][channel] = dither_lines[col][1][channel];
                dither_lines[col][1][channel] = 0;
            }
        }

        // skip overscan (all bitmap rows need to be aligned to 4 byte lengths)
        if (overscan > 0)
            f_lseek(&fil, f_tell(&fil) + overscan);

        watchdog_update();
    }
    printf("close file\n");
    f_close(&fil);

    return true;
}


