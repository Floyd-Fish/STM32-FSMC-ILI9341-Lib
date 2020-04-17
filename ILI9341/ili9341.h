#ifndef __ILI9341_H__
#define __ILI9341_H__

#include "fonts.h"
#include <stdbool.h>

#define ILI9341_CMD_ADDR ( ( uint32_t ) 0x60000000 )    //  Base data

/* Change this value below if you have multiple devices on FSMC interface
 * and you're not using A16 for ILI9341, e.g:
 * 0x600000000 |  (0x1<<(16+1)) = 0x60020000;
 * And if it's A17, then 0x600000000 | (0x1<<(17+1)) = 0x60040000;
 */
#define ILI9341_DATA_ADDR ( ( uint32_t ) 0x60020000 )   //  LCD Register Select: A16

//  Basic functions to write data via 16-bit parallel interface (FSMC)
#define ILI9341_WRITE_REG ( *(__IO uint16_t *) (ILI9341_CMD_ADDR) )
#define ILI9341_WRITE_DATA ( *(__IO uint16_t *) (ILI9341_DATA_ADDR) )

/********************************************************
 * Pin Maps for LCD and STM32:
 * STM32        |       ILI9341_LCD     |       Comment
 * FSMC_Dx     -->      DBx             |   (16 lines in total)
 * FSMC_NOE    -->      LCD_RD          |   Read
 * FSMC_NWE    -->      LCD_WR          |   Write
 * FSMC_NEx    -->      LCD_CS          |   Chip select
 * FSMC_Ax     -->      LCD_RS          |   Register select (NOT Reset!)
 * User defined reset pin  --> LCD_RST  |   Reset   (See below)
 ********************************************************/

//  Reset pin is still needed for reset purpose
#define ILI9341_RES_Pin       GPIO_PIN_5
#define ILI9341_RES_GPIO_Port GPIOB

//  Orientation params
#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

/* Select a rotate direction for display */
#define ILI9341_SET_ROTATION 1

// default orientation
#if ILI9341_SET_ROTATION == 0
    #define ILI9341_WIDTH  240
    #define ILI9341_HEIGHT 320
    #define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR)


// rotate right
#elif ILI9341_SET_ROTATION == 1
    #define ILI9341_WIDTH  320
    #define ILI9341_HEIGHT 240
    #define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)


// rotate left
#elif ILI9341_SET_ROTATION == 2
    #define ILI9341_WIDTH  320
    #define ILI9341_HEIGHT 240
    #define ILI9341_ROTATION (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)

// upside down
#elif ILI9341_SET_ROTATION == 3
    #define ILI9341_WIDTH  240
    #define ILI9341_HEIGHT 320
    #define ILI9341_ROTATION (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR)
#endif

/****************************/

// Color definitions
#define	ILI9341_BLACK   0x0000
#define	ILI9341_BLUE    0x001F
#define	ILI9341_RED     0xF800
#define	ILI9341_GREEN   0x07E0
#define ILI9341_CYAN    0x07FF
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_YELLOW  0xFFE0
#define ILI9341_WHITE   0xFFFF
#define ILI9341_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

#define ABS(x) ((x) > 0 ? (x) : -(x))

/**
 * @brief Initialize the ili9341 controller
 * @param none
 * @return none
 */
void ILI9341_Init(void);

/**
 * @brief Draw a pixel on screen
 * @param x, y -> Coordinates to draw
 * @param color -> color to draw
 * @return none
 */
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Write string to screen
 * @param x, y -> Coordinates to write
 * @param str -> string to write
 * @param font -> font to use
 * @param color -> color of the string
 * @param bgcolor -> Background color of the string
 * @return none
 */
void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);

/**
 * @brief Fill a Rect-Area with single color
 * @param x, y -> Coordinates to start
 * @param w, h -> Width & Height of the Rect.
 * @param color -> color of the Rect.
 * @return none
 */
void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/**
 * @brief Fill screen with single color
 * @param color -> color to fill with
 * @return none
 */
void ILI9341_FillScreen(uint16_t color);

/**
 * @brief Draw a line with single color
 * @param x1&y1 -> coordinate of the start point
 * @param x2&y2 -> coordinate of the end point
 * @param color -> color of the line to Draw
 * @return none
 */
void ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                      uint16_t color);

/**
 * @brief Draw a circle with single color
 * @param x0&y0 -> coordinate of circle center
 * @param r -> radius of circle
 * @param color -> color of circle line
 * @return  none
 */
void ILI9341_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);

/**
 * @brief Draw a Filled circle with single color
 * @param x0&y0 -> coordinate of circle center
 * @param r -> radius of circle
 * @param color -> color of circle
 * @return  none
 */
void ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

/**
 * @brief Draw a Triangle with single color
 * @param  xi&yi -> 3 coordinates of 3 top points.
 * @param color ->color of the lines
 * @return  none
 */
void ILI9341_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                            uint16_t x3, uint16_t y3, uint16_t color);

/**
 * @brief Draw a filled Triangle with single color
 * @param  xi&yi -> 3 coordinates of 3 top points.
 * @param color ->color of the triangle
 * @return  none
 */
void ILI9341_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                                uint16_t x3, uint16_t y3, uint16_t color);

/**
 * @brief Draw an image on the screen
 * @param x, y -> Coordinate of the image's top-left dot (where to start)
 * @param w, h -> Width & Height of the image
 * @param data -> Must be '(uint16_t *)data' ,the image data array
 * @return none
 */
void ILI9341_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);

/**
 * @brief Invert screen color
 * @param invert -> Invert or not
 * @return none
 */
void ILI9341_InvertColors(bool invert);

/**
 * @brief Simple test function for almost all functions
 * @param none
 * @return none
 */
void ILI9341_Test(void);

/**
 * @brief Simple FPS test function (not so reliable...)
 * @param none
 * @return none
 */
void FPS_Test(void);


#endif // __ILI9341_H__
