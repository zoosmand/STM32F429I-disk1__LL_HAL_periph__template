/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
// #include "main.h"
#include "stm32f4xx_it.h"



/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void) {
  //
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void) {
  while (1) {
    //
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void) {
  while (1) {
    //
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void) {
  while (1) {
    //
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void) {
  while (1) {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void) {
  //
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void) {
  //
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void) {
  //
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void) {
  // HAL_IncTick();
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles RCC global interrupt.
  */
void RCC_IRQHandler(void) {
  //
}



/**
  * @brief This function handles EXTI0 global interrupt.
  */
void EXTI0_IRQHandler(void) {
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
  FLAG_SET(_EREG_, _EWUPF_);
}



/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void) {
  LL_USART_ClearFlag_RXNE(USART1);
  // FLAG_SET(_USARTREG_, _RXITF_);
}



/**
  * @brief This function handles RTC alarms A and B interrupt through EXTI line 17.
  */
void RTC_Alarm_IRQHandler(void) {
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_17);

  if (LL_RTC_IsActiveFlag_ALRA(RTC)) {
    LL_RTC_ClearFlag_ALRA(RTC);
    FLAG_SET(_EREG_, _RTCALAF_);
  }

  if (LL_RTC_IsActiveFlag_ALRB(RTC)) {
    LL_RTC_ClearFlag_ALRB(RTC);
    FLAG_SET(_EREG_, _RTCALBF_);
  }
}



/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void) {
  LL_TIM_ClearFlag_UPDATE(TIM7);
  FLAG_SET(_EREG_, _BT7F_);
}



/**
  * @brief This function handles EXTI15_10 global interrupt.
  */
void EXTI15_10_IRQHandler(void) {
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_15)) {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_15);
    FLAG_SET(_EREG_, _ETSF_);
  }
}



/**
  * @brief This function handles DMA2D global interrupt.
  */
void DMA2D_IRQHandler(void) {
  // LL_DMA2D_ClearFlag_CE(DMA2D);

} 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
