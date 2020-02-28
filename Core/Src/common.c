/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Global variables ----------------------------------------------------------*/
uint32_t delay_tmp = 0;

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/



__STATIC_INLINE uint32_t ITM_SendCharChannel(uint32_t ch, uint32_t channel);

////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
  while (1) {
    //
  }
}



/**
  * @brief  This function blink a LED.
  * @retval None
  */
void LED_Blink(GPIO_TypeDef* port, uint16_t pinSource) {
  (PIN_LEVEL(port, pinSource)) ? PIN_L(port, pinSource) : PIN_H(port, pinSource);
}











/********************************************************************************/
/*                         printf() output supply block                         */
/********************************************************************************/
/*
 *
 */
__STATIC_INLINE uint32_t ITM_SendCharChannel(uint32_t ch, uint32_t channel) {
   /* ITM enabled and ITM Port enabled */
  if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && ((ITM->TER & (1 << channel)) != 0UL)) {
    while (ITM->PORT[channel].u32 == 0UL) {
      __NOP();
    }
    ITM->PORT[channel].u8 = (uint8_t)ch;
  }
  return (ch);
}


/*
 *
 */
int _write(int32_t file, char *ptr, int32_t len) {
 for(int32_t i = 0 ; i < len ; i++) {
   ITM_SendCharChannel(*ptr, 0);
   
  //  LL_USART_TransmitData8(USART1, (*ptr));
  //  while (!(LL_USART_IsActiveFlag_TXE(USART1)));
   
   ptr++;
 }
	return len;
}