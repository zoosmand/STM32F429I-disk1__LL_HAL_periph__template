/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_crc.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_iwdg.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_dma2d.h"
#include "stm32f4xx_ll_rtc.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "common.h"
#include "tim.h"
#include "rtc.h"
#include "spi.h"
#include "sdram.h"
#include "dma2d.h"
#include "ltdc.h"
#include "fonts.h"
#include "display.h"
#include "exti.h"
#include "i2c.h"
#include "ts.h"


/* Exported types ------------------------------------------------------------*/
extern uint32_t sysQuantum;
extern uint32_t millis;
extern uint32_t seconds;
extern uint32_t minutes;
extern uint32_t _EREG_;

/* Exported constants --------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define CCMRAM  __attribute__((section(".ccmram")))

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
extern void Delay_Handler(uint32_t delay);
extern void Delay(uint32_t delay);
extern void Cron_Handler(void);
extern void Set_BitBandVal(uint32_t addr, uint32_t key);
extern uint32_t Get_BitBandVal(uint32_t addr);


/* Private defines -----------------------------------------------------------*/
// _EREG_ Flags    
#define _BT7F_    0 // Basic Timer7 Flag
#define _RTCALAF_ 1 // RTC Alarm A Flag
#define _RTCALBF_ 2 // RTC Alarm B Flag
#define _RTWUPF_  3 // RTC Wake Up Flag
#define _RDF_     4 // Run Display Flag
#define _DDF_     5 // Display Delay Flag
#define _DBLF_    6 // Data Buffer is Locked Flag
#define _USART1F_ 7 // USART1 Interrupt occurs Flag
#define _BLINKF_  8 // Blink Flaf
#define _DELAYF_  9 // Delay Flag
#define _EWUPF_   10 // EXTI WakeUp PA0 Flag
#define _ETSF_    11 // EXTI Touch Screen PA15 Flag

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

