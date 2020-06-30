/* Includes ------------------------------------------------------------------*/
#include "exti.h"



/* Private variables ---------------------------------------------------------*/
static uint16_t wupCounter = 0;
static uint16_t tapCounter = 0;
static char tapStr[7];
static Font_TypeDef font_dot20x28 = {
  .width = 24,
  .height = 32,
  .color = ARGB8888_White,
  .bgcolor = ARGB8888_Red,
  .use_bg = 1,
  .font = (uint8_t*)font_20x28
};
static uint16_t xStart = 0;
static uint16_t yStart = 0;










////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  A button press handler, counter of press print out to display
  * @param  None
  * @retval status of a button press
  */
uint8_t WakeUp_Handler(void) {
  uint8_t status = 0;
  wupCounter++;
  
  if (PIN_LEVEL(GPIOA, GPIO_BSRR_BS0_Pos)) {
    if (wupCounter > 50000) {
      wupCounter = 0;
      sprintf(tapStr, "%i", tapCounter++);
      xStart = 40;
      yStart = 40;
      PrintString(L1, &xStart, &yStart, &font_dot20x28, tapStr, 0);
      status = 1;

      TouchScreen_Handler();
    }
  } else {
    wupCounter = 0;
  }
  return (status);
}
