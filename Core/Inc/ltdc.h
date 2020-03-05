#ifndef __LTDC_H
#define __LTDC_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private macro -------------------------------------------------------------*/
#define LCD_CS_H                    PIN_H(GPIOC, GPIO_BSRR_BS2_Pos)
#define LCD_CS_L                    PIN_L(GPIOC, GPIO_BSRR_BS2_Pos)
#define LCD_WRX_H                   PIN_H(GPIOD, GPIO_BSRR_BS13_Pos)
#define LCD_WRX_L                   PIN_L(GPIOD, GPIO_BSRR_BS13_Pos)

#define L1                          1
#define L2                          2

/* Private defines -----------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////
#define ILI9341_MAX_X               (uint16_t)240
#define ILI9341_MAX_Y               (uint16_t)320

// -------- Portrait ---------
// #define ILI9341_HEIGHT              (uint16_t)(ILI9341_MAX_Y - 20)
// #define ILI9341_WIDTH               (uint16_t)(ILI9341_MAX_X - 20)
// -------- Landscape --------
#define ILI9341_HEIGHT              (uint16_t)(ILI9341_MAX_X - 20)
#define ILI9341_WIDTH               (uint16_t)(ILI9341_MAX_Y - 20)

#define ILI9341_BYTES               (uint32_t)(ILI9341_HEIGHT * ILI9341_WIDTH * 4)
#define ILI9341_L1_ADDR             SDRAM_START_ADDR
#define ILI9341_L2_ADDR             (ILI9341_L1_ADDR + ILI9341_BYTES)

//////////////////////////////////////////////////////////////////////////
#define ILI9341_NOP                 0x00
#define ILI9341_SOFTRESET           0x01
#define ILI9341_SLEEPIN             0x10
#define ILI9341_SLEEPOUT            0x11
#define ILI9341_NORMALDISP          0x13
#define ILI9341_INVERTOFF           0x20
#define ILI9341_INVERTON            0x21
#define ILI9341_GAMMASET            0x26
#define ILI9341_DISPLAYOFF          0x28
#define ILI9341_DISPLAYON           0x29
#define ILI9341_CASET               0x2a
#define ILI9341_PASET               0x2b
#define ILI9341_RAMWR               0x2c
#define ILI9341_RAMRD               0x2e
#define ILI9341_MEMCONTROL          0x36
#define ILI9341_PIXELFORMAT         0x3a
#define ILI9341_RGBSIGNAL           0xb0
#define ILI9341_FRAMECONTROL        0xb1
#define ILI9341_INVERSIONCONRTOL    0xb4
#define ILI9341_DISPLAYFUNC         0xb6
#define ILI9341_ENTRYMODE           0xb7
#define ILI9341_POWERCONTROL1       0xc0
#define ILI9341_POWERCONTROL2       0xc1
#define ILI9341_VCOMCONTROL1        0xc5
#define ILI9341_VCOMCONTROL2        0xc7
#define ILI9341_POWERCONTROLA       0xcb
#define ILI9341_POWERCONTROLB       0xcf
#define ILI9341_POSGAMMACORR        0xe0
#define ILI9341_NEGGAMMACORR        0xe1
#define ILI9341_DRIVERTIMINGA       0xe8
#define ILI9341_DRIVERTIMINGB       0xea
#define ILI9341_POWERONSEQ          0xed
#define ILI9341_3DGAMMAENABLE       0xf2
#define ILI9341_INTERFACECONTROL    0xf6
#define ILI9341_PUMPRATIOCONTROL    0xf7

// MADCTL Control Bit Definition
#define ILI9341_MADCTL_MY           0x80
#define ILI9341_MADCTL_MX           0x40
#define ILI9341_MADCTL_MV           0x20
#define ILI9341_MADCTL_ML           0x10
#define ILI9341_MADCTL_BGR          0x08
#define ILI9341_MADCTL_MH           0x04










/* Exported functions prototypes ---------------------------------------------*/
uint8_t LTDC_Init(LTDC_TypeDef *device, LTDC_InitTypeDef *init);
void LTDC_SetConfig(LTDC_TypeDef *device, LTDC_LayerCfgTypeDef *layerCfg, uint32_t layerIdx);
void LTDC_Enable(LTDC_TypeDef *device);

void LCD_InitCmd(uint8_t cmd);
void LCD_InitData(uint8_t data);
void ILI9341_Init(void);

void ILI9341_FillLayer(uint8_t layer, uint32_t color);



#ifdef __cplusplus
}
#endif

#endif /* __LTDC_H */
