/**
  ******************************************************************************
  * File Name          : TIM.h
  * Description        : 
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
  
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"



/* Exported functions prototypes ---------------------------------------------*/
void BasicTimer_FromIT_Handler(TIM_TypeDef* tim);



#ifdef __cplusplus
}
#endif
#endif /*__TIM_H */

