/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : 
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"










////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Basic Timer Handler called from interrupt via flag's handler
  * @param  tim: pointer to a basic timer instance
  * @retval status of a button press
  */
void BasicTimer_FromIT_Handler(TIM_TypeDef* tim) {
  if (tim == TIM7) {
    //
  }
}
