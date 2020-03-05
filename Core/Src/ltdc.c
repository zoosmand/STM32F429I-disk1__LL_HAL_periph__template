/* Includes ------------------------------------------------------------------*/
#include "ltdc.h"









////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Initialize the LTDC according to the specified parameters in the LTDC_InitTypeDef.
  * @param  device:  pointer to a LTDC_TypeDef 
  * @param  init:  pointer to a LTDC_InitTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval init status
  */
uint8_t LTDC_Init(LTDC_TypeDef *device, LTDC_InitTypeDef *init) {
  uint32_t tmp, tmp1;

  /* Change LTDC peripheral state */
  // hltdc->State = HAL_LTDC_STATE_BUSY;

  /* Configure the HS, VS, DE and PC polarity */
  device->GCR &= ~(LTDC_GCR_HSPOL | LTDC_GCR_VSPOL | LTDC_GCR_DEPOL | LTDC_GCR_PCPOL);
  device->GCR |= (uint32_t)(
      init->HSPolarity
    | init->VSPolarity
    | init->DEPolarity
    | init->PCPolarity
  );

  /* Set Synchronization size */
  device->SSCR &= ~(LTDC_SSCR_VSH | LTDC_SSCR_HSW);
  tmp = (init->HorizontalSync << 16U);
  device->SSCR |= (tmp | init->VerticalSync);

  /* Set Accumulated Back porch */
  device->BPCR &= ~(LTDC_BPCR_AVBP | LTDC_BPCR_AHBP);
  tmp = (init->AccumulatedHBP << 16U);
  device->BPCR |= (tmp | init->AccumulatedVBP);

  /* Set Accumulated Active Width */
  device->AWCR &= ~(LTDC_AWCR_AAH | LTDC_AWCR_AAW);
  tmp = (init->AccumulatedActiveW << 16U);
  device->AWCR |= (tmp | init->AccumulatedActiveH);

  /* Set Total Width */
  device->TWCR &= ~(LTDC_TWCR_TOTALH | LTDC_TWCR_TOTALW);
  tmp = (init->TotalWidth << 16U);
  device->TWCR |= (tmp | init->TotalHeigh);

  /* Set the background color value */
  tmp = ((uint32_t)(init->Backcolor.Green) << 8U);
  tmp1 = ((uint32_t)(init->Backcolor.Red) << 16U);
  device->BCCR &= ~(LTDC_BCCR_BCBLUE | LTDC_BCCR_BCGREEN | LTDC_BCCR_BCRED);
  device->BCCR |= (tmp1 | tmp | init->Backcolor.Blue);

  /* Enable the Transfer Error and FIFO underrun interrupts */
  device->IER |= (LTDC_IT_TE | LTDC_IT_FU);
  // __HAL_LTDC_ENABLE_IT(hltdc, LTDC_IT_TE | LTDC_IT_FU);

  return (1);
}




/**
  * @brief  Start LTDC.
  * @param  device:  pointer to a LTDC_TypeDef 
  * @retval init status
  */
void LTDC_Enable(LTDC_TypeDef *device) {
  /* Enable Dither by setting DTEN bit */
  device->GCR |= (uint32_t)LTDC_GCR_DEN;

  /* Enable LTDC by setting LTDCEN bit */
  device->GCR |= LTDC_GCR_LTDCEN;
}




/**
  * @brief  Setup an LTDC Layer according to the specified parameters in the LTDC_LayerCfgTypeDef.
  * @param  device:  pointer to a LTDC_TypeDef 
  * @param  layerCfg:  pointer to a LTDC_LayerCfgTypeDef 
  * @param  layerIdx:  number of a Layer
  * @retval None
  */
void LTDC_SetConfig(LTDC_TypeDef *device, LTDC_LayerCfgTypeDef *layerCfg, uint32_t layerIdx) {
  uint32_t tmp;
  uint32_t tmp1;
  uint32_t tmp2;

  LTDC_Layer_TypeDef* layer;
  layer = (LTDC_Layer_TypeDef*)((layerIdx == 2) ? LTDC_Layer2_BASE : LTDC_Layer1_BASE);

  /* Configure the horizontal start and stop position */
  tmp = ((layerCfg->WindowX1 + ((device->BPCR & LTDC_BPCR_AHBP) >> 16U)) << 16U);
  layer->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
  layer->WHPCR = ((layerCfg->WindowX0 + ((device->BPCR & LTDC_BPCR_AHBP) >> 16U) + 1U) | tmp);

  /* Configure the vertical start and stop position */
  tmp = ((layerCfg->WindowY1 + (device->BPCR & LTDC_BPCR_AVBP)) << 16U);
  layer->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
  layer->WVPCR  = ((layerCfg->WindowY0 + (device->BPCR & LTDC_BPCR_AVBP) + 1U) | tmp);

  /* Specifies the pixel format */
  layer->PFCR &= ~(LTDC_LxPFCR_PF);
  layer->PFCR = (layerCfg->PixelFormat);

  /* Configure the default color values */
  tmp = ((uint32_t)(layerCfg->Backcolor.Green) << 8U);
  tmp1 = ((uint32_t)(layerCfg->Backcolor.Red) << 16U);
  tmp2 = (layerCfg->Alpha0 << 24U);
  layer->DCCR &= ~(LTDC_LxDCCR_DCBLUE | LTDC_LxDCCR_DCGREEN | LTDC_LxDCCR_DCRED | LTDC_LxDCCR_DCALPHA);
  layer->DCCR = (layerCfg->Backcolor.Blue | tmp | tmp1 | tmp2);

  /* Specifies the constant alpha value */
  layer->CACR &= ~(LTDC_LxCACR_CONSTA);
  layer->CACR = (layerCfg->Alpha);

  /* Specifies the blending factors */
  layer->BFCR &= ~(LTDC_LxBFCR_BF2 | LTDC_LxBFCR_BF1);
  layer->BFCR = (layerCfg->BlendingFactor1 | layerCfg->BlendingFactor2);

  /* Configure the color frame buffer start address */
  layer->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
  layer->CFBAR = (layerCfg->FBStartAdress);

  if (layerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888) {
    tmp = 4U;
  } else if (layerCfg->PixelFormat == LTDC_PIXEL_FORMAT_RGB888) {
    tmp = 3U;
  } else if ((layerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
           (layerCfg->PixelFormat == LTDC_PIXEL_FORMAT_RGB565)   || \
           (layerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB1555) || \
           (layerCfg->PixelFormat == LTDC_PIXEL_FORMAT_AL88)) {
    tmp = 2U;
  } else {
    tmp = 1U;
  }

  /* Configure the color frame buffer pitch in byte */
  layer->CFBLR  &= ~(LTDC_LxCFBLR_CFBLL | LTDC_LxCFBLR_CFBP);
  layer->CFBLR  = (((layerCfg->ImageWidth * tmp) << 16U) | (((layerCfg->WindowX1 - layerCfg->WindowX0) * tmp)  + 3U));
  /* Configure the frame buffer line number */
  layer->CFBLNR  &= ~(LTDC_LxCFBLNR_CFBLNBR);
  layer->CFBLNR  = (layerCfg->ImageHeight);

  /* Enable LTDC_Layer by setting LEN bit */
  layer->CR |= (uint32_t)LTDC_LxCR_LEN;

  /* Set the Immediate Reload type */
  device->SRCR = LTDC_SRCR_IMR;
}




/**
  * @brief  Send an initialization command to display.
  * @param  cmd: command
  * @retval None
  */
 void LCD_InitCmd(uint8_t cmd) {
  LCD_WRX_L;
  LCD_CS_L;
  SPI_Write8b(SPI5, cmd);
  Delay(5);
  LCD_CS_H;
}




/**
  * @brief  Send an initialization data to display.
  * @param  data: data
  * @retval None
  */
void LCD_InitData(uint8_t data) {
  LCD_WRX_H;
  LCD_CS_L;
  SPI_Write8b(SPI5, data);
  Delay(5);
  LCD_CS_H;
}




/**
  * @brief  Send the sequence of initialization command and data to display.
  * @retval None
  */
void ILI9341_Init(void) {
  LCD_InitCmd(ILI9341_SOFTRESET); // 0x01
  Delay(20);

  LCD_InitCmd(ILI9341_DISPLAYOFF); // 0x28
  Delay(20);
 
  // // ----------------- Power ------------------
  // LCD_InitCmd(ILI9341_POWERCONTROL1); // 0xc0
  // LCD_InitData(0x26);
  // LCD_InitCmd(ILI9341_POWERCONTROL2); // 0xc1
  // LCD_InitData(0x11);
  // LCD_InitCmd(ILI9341_VCOMCONTROL1); // 0xc5
  // LCD_InitData(0x5c);
  // LCD_InitData(0x4c);
  // LCD_InitCmd(ILI9341_VCOMCONTROL2); // 0xc7
  // LCD_InitData(0x94);
  
  // ---------------- Memory ------------------
  LCD_InitCmd(ILI9341_MEMCONTROL); // 0x36
  // -------- Portrait ---------
  //ILI9341_WriteData8(ILI9341_MADCTL_BGR | ILI9341_MADCTL_MV | ILI9341_MADCTL_MX | ILI9341_MADCTL_MY);
  // -------- Landscape --------
  LCD_InitData(ILI9341_MADCTL_BGR | ILI9341_MADCTL_MY);
  
  LCD_InitCmd(ILI9341_PIXELFORMAT); // 0x3a
  LCD_InitData(0x55);

  // // -------------- Frame rate ---------------
  // LCD_InitCmd(ILI9341_FRAMECONTROL); // 0xb1
  // LCD_InitData(0x00);
  // LCD_InitData(0x1b);
  
  // // ---------------- Gamma ------------------
  // LCD_InitCmd(ILI9341_GAMMASET); // 0x26
  // LCD_InitData(0x01);
  // LCD_InitCmd(ILI9341_POSGAMMACORR); // 0xe0
  // LCD_InitData(0x1f);
  // LCD_InitData(0x1a);
  // LCD_InitData(0x18);
  // LCD_InitData(0x0a);
  // LCD_InitData(0x0f);
  // LCD_InitData(0x06);
  // LCD_InitData(0x45);
  // LCD_InitData(0x87);
  // LCD_InitData(0x32);
  // LCD_InitData(0x0a);
  // LCD_InitData(0x07);
  // LCD_InitData(0x02);
  // LCD_InitData(0x07);
  // LCD_InitData(0x05);
  // LCD_InitData(0x00);
  // LCD_InitCmd(ILI9341_NEGGAMMACORR); // 0xe1
  // LCD_InitData(0x00);
  // LCD_InitData(0x25);
  // LCD_InitData(0x27);
  // LCD_InitData(0x05);
  // LCD_InitData(0x10);
  // LCD_InitData(0x09);
  // LCD_InitData(0x3a);
  // LCD_InitData(0x78);
  // LCD_InitData(0x4d);
  // LCD_InitData(0x05);
  // LCD_InitData(0x18);
  // LCD_InitData(0x0d);
  // LCD_InitData(0x38);
  // LCD_InitData(0x3a);
  // LCD_InitData(0x1f);
  
  // // ---------------- DDRAM -------------------  
  // LCD_InitCmd(ILI9341_CASET); // 0x2a
  // LCD_InitData(0x00);
  // LCD_InitData(0x00);
  // LCD_InitData(0x00);
  // LCD_InitData(0xef);
  // LCD_InitCmd(ILI9341_PASET); // 0x2b
  // LCD_InitData(0x00);
  // LCD_InitData(0x00);
  // LCD_InitData(0x01);
  // LCD_InitData(0x3f);
  // LCD_InitCmd(ILI9341_RAMWR); // 0x2c
  // LCD_InitCmd(ILI9341_ENTRYMODE); // 0xb7
  // LCD_InitData(0x07);

  // --------------- Display -----------------
  LCD_InitCmd(ILI9341_DISPLAYFUNC); // 0xb6
  LCD_InitData(0x0a);
  LCD_InitData(0x82);
  LCD_InitData(0x27);
  LCD_InitData(0x00);
  // LCD_InitCmd(ILI9341_INVERTON);

  LCD_InitCmd(ILI9341_INTERFACECONTROL); // 0xf6
  LCD_InitData(0x01);
  LCD_InitData(0x00);
  LCD_InitData(0x06);

  LCD_InitCmd(ILI9341_SLEEPOUT); // 0x11
  Delay(20);
  LCD_InitCmd(ILI9341_DISPLAYON); // 0x29
}


