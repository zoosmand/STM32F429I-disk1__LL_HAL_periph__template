#ifndef __DISPLAY_H
#define __DISPLAY_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct {
  uint8_t     width;
  uint8_t     height;
  uint32_t    color;
  uint32_t    bgcolor;
  uint8_t     use_bg;
  uint8_t     *font;
} Font_TypeDef;



/* Exported macro ------------------------------------------------------------*/
#define L1_ADDRESS(x, y)  (ILI9341_L1_ADDR + (x * ILI9341_HEIGHT * 4) + (y * 4))
#define L2_ADDRESS(x, y)  (ILI9341_L2_ADDR + (x * ILI9341_HEIGHT * 4) + (y * 4))


/* Exported functions prototypes ---------------------------------------------*/
void FillLayer(uint8_t layer, uint32_t color);
void FillRectangle(uint8_t layer, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);
void DrawPixel(uint8_t layer, uint16_t x, uint16_t y, uint32_t color);
void DrawVLine(uint8_t layer, uint16_t x, uint16_t y, uint16_t h, uint32_t color);
void DrawHLine(uint8_t layer, uint16_t x, uint16_t y, uint16_t w, uint32_t color);
void DrawSymbol(uint8_t layer, uint16_t *x, uint16_t *y, const Font_TypeDef *font, uint8_t pos);
void PrintString(uint8_t layer, uint16_t *x, uint16_t *y, const Font_TypeDef *font, const char *buf, uint8_t wrap);
void PrintDate(uint8_t layer, const char* buf);
void PrintTime(uint8_t layer, const char* buf);







#ifdef __cplusplus
}
#endif

#endif /* __DISPLAY_H */
