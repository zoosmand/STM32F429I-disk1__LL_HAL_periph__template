/* Includes ------------------------------------------------------------------*/
#include "display.h"


/* Private variables ---------------------------------------------------------*/
static Font_TypeDef font_dot15x21 = {
  .width    = 18,
  .height   = 24,
  .color    = ARGB8888_White,
  .bgcolor  = ARGB8888_Darkgreen,
  .use_bg   = 1,
  .font     = (uint8_t*)font_15x21
};

static Font_TypeDef font_dot20x28 = {
  .width    = 24,
  .height   = 32,
  .color    = ARGB8888_White,
  .bgcolor  = ARGB8888_Darkgreen,
  .use_bg   = 1,
  .font =   (uint8_t*)font_20x28
};

static uint16_t xStart = 0;
static uint16_t yStart = 0;










////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Fill a layer of display with color
  * @param  layer: number of layer
  * @param  color: color
  * @retval None
  */
void FillLayer(uint8_t layer, uint32_t color) {
  uint32_t layerAddr = (layer == L2) ? ILI9341_L2_ADDR : ILI9341_L1_ADDR;
  for (uint32_t i = 0; i < ILI9341_BYTES; i += 4) {
    *(__IO uint32_t*)(layerAddr + i) = color;
  }
}




/**
  * @brief  Draw a pixel on a layer of display with color
  * @param  layer: number of layer
  * @param  x: x coordinate
  * @param  y: y coordinate 
  * @param  color: color
  * @retval None
  */
void DrawPixel(uint8_t layer, uint16_t x, uint16_t y, uint32_t color) {
  *(__IO uint32_t*)((layer == L2) ? L2_ADDRESS(x, y) : L1_ADDRESS(x, y)) = color;
}




/**
  * @brief  Draw a vertical line on a layer of display with color
  * @param  layer: number of layer
  * @param  x: x coordinate
  * @param  y: y coordinate 
  * @param  h: height of a line
  * @param  color: color
  * @retval None
  */
void DrawVLine(uint8_t layer, uint16_t x, uint16_t y, uint16_t h, uint32_t color) {
  if ((x >= ILI9341_HEIGHT) || (y >= ILI9341_WIDTH)) return;
  if ((y + h - 1) >= ILI9341_HEIGHT)  h = ILI9341_HEIGHT - y;

  uint32_t layerAddr = (uint32_t)((layer == L2) ? L2_ADDRESS(x, y) : L1_ADDRESS(x, y));
  h <<= 2;
  while (h--) {
    *(__IO uint32_t*)(layerAddr++) = color;
  }

}





/**
  * @brief  Draw a horizontal line on a layer of display with color
  * @param  layer: number of layer
  * @param  x: x coordinate
  * @param  y: y coordinate 
  * @param  w: weight of a line
  * @param  color: color
  * @retval None
  */
void DrawHLine(uint8_t layer, uint16_t x, uint16_t y, uint16_t w, uint32_t color) {
  if ((x >= ILI9341_HEIGHT) || (y >= ILI9341_WIDTH)) return;
  if ((x + w - 1) >= ILI9341_WIDTH)  w = ILI9341_WIDTH - x;

  uint32_t layerAddr = (uint32_t)((layer == L2) ? L2_ADDRESS(x, y) : L1_ADDRESS(x, y));

  uint32_t hh = ILI9341_HEIGHT << 2;
  while (w--) {
    *(__IO uint32_t*)(layerAddr) = color;
    layerAddr += hh;
  }

}





/**
  * @brief  Fill rectangle on a layer of display with color
  * @param  layer: number of layer
  * @param  x: x coordinate
  * @param  y: y coordinate 
  * @param  h: height of a rectangle
  * @param  w: weight of a rectangle
  * @param  color: color
  * @retval None
  */
void FillRectangle(uint8_t layer, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color) {
  if ((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT) || (w == 0) || (h == 0)) return;
  if ((x + w - 1) >= ILI9341_WIDTH)  w = ILI9341_WIDTH  - x;
  if ((y + h - 1) >= ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;

  uint32_t layerAddr = (uint32_t)((layer == L2) ? L2_ADDRESS(x, y) : L1_ADDRESS(x, y));
  uint16_t hh = 0;
  uint32_t ww = ((ILI9341_HEIGHT << 2) - (h << 2));

  while (w--) {
    hh = h;
    hh <<= 2;
    while (hh--) {
      *(__IO uint32_t*)(layerAddr++) = color;
    }
    layerAddr += ww;
  }
}




/**
  * @brief  Draw symbol on a layer of display
  * @param  layer: number of layer
  * @param  x: pointer to x coordinate
  * @param  y: pointer to y coordinate 
  * @param  font: pointer to font structure containes font paramenters:
  *               - width of a symbol
  *               - height of a symbol
  *               - color of a symbol
  *               - background color behind a symbol
  *               - flag - use or not use background color
  *               - array of symbols
  * @param  pos: position in the sysmbol's array
  * @retval None
  */
void DrawSymbol(uint8_t layer, uint16_t *x, uint16_t *y, const Font_TypeDef *font, uint8_t pos) {
  if ((pos > 126) || (pos < 32)) {
    if (pos == 176) pos = 95;
    else return;
  } else {
    pos -= 32;
  }

  int16_t w = font->width;
  int16_t h = font->height;
  if ((*x >= ILI9341_WIDTH) || (*y >= ILI9341_HEIGHT) || (w == 0) || (h == 0)) return;
  if ((*x + w - 1) >= ILI9341_WIDTH)  w = ILI9341_WIDTH  - *x;
  if ((*y + h - 1) >= ILI9341_HEIGHT) h = ILI9341_HEIGHT - *y;

  uint8_t smb = 0;
  uint8_t z = 0;
  uint8_t *fontAddr = font->font;
  
  uint32_t layerAddr = (uint32_t)((layer == L2) ? L2_ADDRESS(*x, *y) : L1_ADDRESS(*x, *y));
  uint16_t hh = 0;
  uint32_t ww = ((ILI9341_HEIGHT << 2) - (h << 2));
  *x += w;

  while (w--) {
    z = font->height / 8;
    while (z--) {
      smb = fontAddr[(pos * font->width * (font->height / 8))];
      fontAddr++;

      hh = 8;
      while (hh--) {
        if (smb & 0x01) {
          *(__IO uint32_t*)layerAddr = font->color;
        } else {
          if (font->use_bg) {
            *(__IO uint32_t*)layerAddr = font->bgcolor;
          }
        }
        layerAddr +=4;
        smb >>= 1;
      }
    }
    layerAddr += ww;
  }
}




/**
  * @brief  Print a string on a layer of display
  * @param  layer: number of layer
  * @param  x: pointer to x coordinate
  * @param  y: pointer to y coordinate 
  * @param  font: pointer to font structure containes font paramenters:
  *               - width of a symbol
  *               - height of a symbol
  *               - color of a symbol
  *               - background color behind a symbol
  *               - flag - use or not use background color
  *               - array of symbols
  * @param  buf: pointer to buffer with a string
  * @param  wrap: wrap or not wrap to the next line 
  * @retval None
  */
void PrintString(uint8_t layer, uint16_t *x, uint16_t *y, const Font_TypeDef *font, const char *buf, uint8_t wrap) {
  while (*buf) {
    if (*x > ILI9341_WIDTH - 20) {
      if (wrap) {
        *x = 20;
        *y -= font->height;
      } else {
        return;
      }
    }
    DrawSymbol(layer, x, y, font, *buf);
    buf++;
  }
}




/**
  * @brief  Print date on a layer of display to certain coordinates
  * @param  layer: number of layer
  * @param  buf: pointer to buffer with date digits
  * @retval None
  */
void PrintDate(uint8_t layer, const char* buf) {
  xStart = 30;
  yStart = 140;

  PrintString(layer, &xStart, &yStart, &font_dot15x21, buf, 0);
}




/**
  * @brief  Print time on a layer of display to certain coordinates
  * @param  layer: number of layer
  * @param  buf: pointer to buffer with time digits
  * @retval None
  */
void PrintTime(uint8_t layer, const char* buf) {
  xStart = 25;
  yStart = 100;

  PrintString(layer, &xStart, &yStart, &font_dot20x28, buf, 0);
}

