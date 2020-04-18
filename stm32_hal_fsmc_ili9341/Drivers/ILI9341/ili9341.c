/* vim: set ai et ts=4 sw=4: */
#include "stm32f4xx_hal.h"
#include "ili9341.h"
#include "string.h"
#include "stdio.h"

/* Static functions for fundamental instructions */
static void ILI9341_Reset() {
    HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_SET);
}

static void ILI9341_WriteCommand(uint16_t cmd) {
    ILI9341_WRITE_REG = cmd;
}

static void ILI9341_WriteData(uint16_t data) {
    ILI9341_WRITE_DATA = data;
}

static void ILI9341_WriteDataMultiple(uint16_t * datas, uint32_t dataNums) {
    while (dataNums--)
    {
        ILI9341_WRITE_DATA = *datas++;
    }
}

static void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // column address set
    ILI9341_WriteCommand(0x2A); // CASET
    {
        ILI9341_WriteData(x0 >> 8);
        ILI9341_WriteData(x0 & 0x00FF);
        ILI9341_WriteData(x1 >> 8);
        ILI9341_WriteData(x1 & 0x00FF);
    }
    // row address set
    ILI9341_WriteCommand(0x2B); // RASET
    {
        ILI9341_WriteData(y0 >> 8);
        ILI9341_WriteData(y0 & 0x00FF);
        ILI9341_WriteData(y1 >> 8);
        ILI9341_WriteData(y1 & 0x00FF);
    }
    // write to RAM
    ILI9341_WriteCommand(0x2C); // RAMWR
}

void ILI9341_Init() {
    ILI9341_Reset();
    // command list is based on https://github.com/martnak/STM32-ILI9341
    // SOFTWARE RESET
    ILI9341_WriteCommand(0x01);
    HAL_Delay(500);
        
    // POWER CONTROL A
    ILI9341_WriteCommand(0xCB);
    {
        uint16_t data[] = { 0x39, 0x2C, 0x00, 0x34, 0x02 };
        ILI9341_WriteDataMultiple(data, 5);
    }

    // POWER CONTROL B
    ILI9341_WriteCommand(0xCF);
    {
        uint16_t data[] = { 0x00, 0xC1, 0x30 };
        ILI9341_WriteDataMultiple(data, 3);
    }

    // DRIVER TIMING CONTROL A
    ILI9341_WriteCommand(0xE8);
    {
        uint16_t data[] = { 0x85, 0x00, 0x78 };
        ILI9341_WriteDataMultiple(data, 3);
    }

    // DRIVER TIMING CONTROL B
    ILI9341_WriteCommand(0x00EA);
    {
        uint16_t data[] = { 0x00, 0x00 };
        ILI9341_WriteDataMultiple(data, 2);
    }

    // POWER ON SEQUENCE CONTROL
    ILI9341_WriteCommand(0xED);
    {
        uint16_t data[] = { 0x04, 0x03, 0x12, 0x81 };
        ILI9341_WriteDataMultiple(data, 4);
    }

    // PUMP RATIO CONTROL
    ILI9341_WriteCommand(0xF7);
    {
        uint16_t data[] = { 0x20 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // POWER CONTROL,VRH[5:0]
    ILI9341_WriteCommand(0xC0);
    {
        uint16_t data[] = { 0x23 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // POWER CONTROL,SAP[2:0];BT[3:0]
    ILI9341_WriteCommand(0xC1);
    {
        uint16_t data[] = { 0x10 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // VCM CONTROL
    ILI9341_WriteCommand(0xC5);
    {
        uint16_t data[] = { 0x3E, 0x28 };
        ILI9341_WriteDataMultiple(data, 2);
    }

    // VCM CONTROL 2
    ILI9341_WriteCommand(0xC7);
    {
        uint16_t data[] = { 0x86 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // MEMORY ACCESS CONTROL
    ILI9341_WriteCommand(0x36);
    {
        uint16_t data[] = { 0x48 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // PIXEL FORMAT
    ILI9341_WriteCommand(0x3A);
    {
        uint16_t data[] = { 0x55 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // FRAME RATIO CONTROL, STANDARD RGB COLOR
    ILI9341_WriteCommand(0xB1);
    {
        uint16_t data[] = { 0x00, 0x18 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // DISPLAY FUNCTION CONTROL
    ILI9341_WriteCommand(0xB6);
    {
        uint16_t data[] = { 0x08, 0x82, 0x27 };
        ILI9341_WriteDataMultiple(data, 3);
    }

    // 3GAMMA FUNCTION DISABLE
    ILI9341_WriteCommand(0xF2);
    {
        uint16_t data[] = { 0x00 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // GAMMA CURVE SELECTED
    ILI9341_WriteCommand(0x26);
    {
        uint16_t data[] = { 0x01 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // POSITIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE0);
    {
        uint16_t data[] = { 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                           0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 };
        ILI9341_WriteDataMultiple(data, 15);
    }

    // NEGATIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE1);
    {
        uint16_t data[] = { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                           0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };
        ILI9341_WriteDataMultiple(data, 15);
    }

    // EXIT SLEEP
    ILI9341_WriteCommand(0x11);
    HAL_Delay(120);

    // TURN ON DISPLAY
    ILI9341_WriteCommand(0x29);

    // MADCTL
    ILI9341_WriteCommand(0x36);
    {
        uint16_t data[] = { ILI9341_ROTATION };
        ILI9341_WriteDataMultiple(data, 1);
    }
}


void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT))
        return;
    ILI9341_SetAddressWindow(x, y, x+1, y+1);
    ILI9341_WriteData(color);
}
/*
 * Some problem occurred...Deserted temporarily
static void ILI9341_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;
    ILI9341_SetAddressWindow(x, y, x+font.width-1, y+font.height-1);
    for(i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++) {
            if( (b << j) & 0x8000)  {
                ILI9341_WriteData(color);
            } else {
                ILI9341_WriteData(bgcolor);
            }
        }
    }
}
 */

static void ILI9341_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
    uint32_t i, b, j;
    ILI9341_SetAddressWindow(x, y, x+font.width-1, y+font.height-1);
    for (i = 0; i < font.height; i++)
    {
        b = font.data[(ch-32) * font.height + i];
        for (j = 0; j < font.width; j++)
        {
            if ((b << j) & 0x8000)
                ILI9341_DrawPixel(x+j, y+i, color);

            else
                ILI9341_DrawPixel(x+j, y+i, bgcolor);
        }
    }
}


void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
    while(*str) {
        if(x + font.width >= ILI9341_WIDTH) {
            x = 0;
            y += font.height;
            if(y + font.height >= ILI9341_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ILI9341_WriteChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }
}


void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // clipping
    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;
    if((x + w - 1) >= ILI9341_WIDTH) w = ILI9341_WIDTH - x;
    if((y + h - 1) >= ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;

    ILI9341_SetAddressWindow(x, y, x+w-1, y+h-1);
    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            ILI9341_WriteData(color);
        }
    }
}


void ILI9341_FillScreen(uint16_t color) {
    ILI9341_FillRectangle(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}

void ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                     uint16_t color) {
    uint16_t swap;
    uint16_t steep = ABS(y1 - y0) > ABS(x1 - x0);
    if (steep) {
        swap = x0;
        x0 = y0;
        y0 = swap;

        swap = x1;
        x1 = y1;
        y1 = swap;
        //_swap_int16_t(x0, y0);
        //_swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        swap = x0;
        x0 = x1;
        x1 = swap;

        swap = y0;
        y0 = y1;
        y1 = swap;
        //_swap_int16_t(x0, x1);
        //_swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = ABS(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            ILI9341_DrawPixel(y0, x0, color);
        } else {
            ILI9341_DrawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void ILI9341_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ILI9341_DrawPixel(x0, y0 + r, color);
    ILI9341_DrawPixel(x0, y0 - r, color);
    ILI9341_DrawPixel(x0 + r, y0, color);
    ILI9341_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9341_DrawPixel(x0 + x, y0 + y, color);
        ILI9341_DrawPixel(x0 - x, y0 + y, color);
        ILI9341_DrawPixel(x0 + x, y0 - y, color);
        ILI9341_DrawPixel(x0 - x, y0 - y, color);

        ILI9341_DrawPixel(x0 + y, y0 + x, color);
        ILI9341_DrawPixel(x0 - y, y0 + x, color);
        ILI9341_DrawPixel(x0 + y, y0 - x, color);
        ILI9341_DrawPixel(x0 - y, y0 - x, color);
    }
}

void ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ILI9341_DrawPixel(x0, y0 + r, color);
    ILI9341_DrawPixel(x0, y0 - r, color);
    ILI9341_DrawPixel(x0 + r, y0, color);
    ILI9341_DrawPixel(x0 - r, y0, color);
    ILI9341_DrawLine(x0 - r, y0, x0 + r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9341_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        ILI9341_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

        ILI9341_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
        ILI9341_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
    }
}

void ILI9341_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                            uint16_t x3, uint16_t y3, uint16_t color)
{
    /* Draw lines */
    ILI9341_DrawLine(x1, y1, x2, y2, color);
    ILI9341_DrawLine(x2, y2, x3, y3, color);
    ILI9341_DrawLine(x3, y3, x1, y1, color);
}

void ILI9341_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                                uint16_t x3, uint16_t y3, uint16_t color)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = ABS(x2 - x1);
    deltay = ABS(y2 - y1);
    x = x1;
    y = y1;

    if (x2 >= x1) {
        xinc1 = 1;
        xinc2 = 1;
    }
    else {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1) {
        yinc1 = 1;
        yinc2 = 1;
    }
    else {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay) {
        xinc1 = 0;
        yinc2 = 0;
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;
    }
    else {
        xinc2 = 0;
        yinc1 = 0;
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;
    }

    for (curpixel = 0; curpixel <= numpixels; curpixel++) {
        ILI9341_DrawLine(x, y, x3, y3, color);

        num += numadd;
        if (num >= den) {
            num -= den;
            x += xinc1;
            y += yinc1;
        }
        x += xinc2;
        y += yinc2;
    }
}

void ILI9341_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data) {
    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;
    if((x + w - 1) >= ILI9341_WIDTH) return;
    if((y + h - 1) >= ILI9341_HEIGHT) return;

    ILI9341_SetAddressWindow(x, y, x+w-1, y+h-1);
    ILI9341_WriteDataMultiple((uint16_t*)data, w*h);
}

void ILI9341_InvertColors(bool invert) {
    ILI9341_WriteCommand(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
}

void ILI9341_Test(void)
{
    ILI9341_FillScreen(ILI9341_WHITE);
    HAL_Delay(1000);
    ILI9341_WriteString(10, 10, "Fuxk you shit", Font_16x26, ILI9341_RED, ILI9341_WHITE);
    HAL_Delay(2000);
    ILI9341_FillScreen(ILI9341_RED);
    HAL_Delay(1000);
    ILI9341_FillScreen(ILI9341_BLUE);
    HAL_Delay(1000);
    ILI9341_FillScreen(ILI9341_GREEN);
    HAL_Delay(1000);
    ILI9341_FillScreen(ILI9341_YELLOW);
    HAL_Delay(1000);
    ILI9341_FillScreen(ILI9341_CYAN);
    HAL_Delay(1000);
    ILI9341_FillScreen(ILI9341_MAGENTA);
    HAL_Delay(1000);
    ILI9341_FillScreen(ILI9341_BLACK);
    HAL_Delay(2000);

    ILI9341_FillRectangle(10, 10, 30, 20, ILI9341_WHITE);
    HAL_Delay(1500);
    ILI9341_FillScreen(ILI9341_BLACK);

    ILI9341_DrawLine(10, 10, 30, 30, ILI9341_WHITE);
    HAL_Delay(1500);
    ILI9341_FillScreen(ILI9341_BLACK);

    ILI9341_DrawCircle(100, 100, 50, ILI9341_WHITE);
    HAL_Delay(1500);
    ILI9341_FillScreen(ILI9341_BLACK);

    ILI9341_DrawFilledCircle(100, 100, 50, ILI9341_WHITE);
    HAL_Delay(1500);
    ILI9341_FillScreen(ILI9341_BLACK);

    ILI9341_DrawTriangle(10, 10, 50, 50, 100, 10, ILI9341_WHITE);
    HAL_Delay(1500);
    ILI9341_FillScreen(ILI9341_BLACK);

    ILI9341_DrawFilledTriangle(10, 10, 50, 50, 100, 10, ILI9341_WHITE);
    HAL_Delay(1500);
    ILI9341_FillScreen(ILI9341_WHITE);
    HAL_Delay(1000);
}

void FPS_Test(void)
{
    ILI9341_FillScreen(ILI9341_WHITE);

    uint32_t start = HAL_GetTick();
    uint32_t end = start;
    int fps = 0;
    char message[] = "ABCDEFGHJK";
    do {
        ILI9341_WriteString(10, 10, message, Font_11x18, ILI9341_RED, ILI9341_WHITE);

        char ch = message[0];
        memmove(message, message+1, sizeof(message)-2);
        message[sizeof(message)-2] = ch;

        fps++;
        end = HAL_GetTick();
    } while ((end - start) < 5000);

    HAL_Delay(1000);
    char buff[64];
    fps = (float) fps / ((end - start) / 1000.0);
    snprintf(buff, sizeof(buff), "~%d FPS", fps);

    ILI9341_FillScreen(ILI9341_WHITE);
    ILI9341_WriteString(10, 10, buff, Font_11x18, ILI9341_RED, ILI9341_WHITE);
    HAL_Delay(3000);
}

