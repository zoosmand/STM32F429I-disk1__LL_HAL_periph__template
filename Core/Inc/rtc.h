/**
  ******************************************************************************
  * File Name          : RTC.h
  * Description        : 
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
  
  
  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"



/* Exported functions prototypes ---------------------------------------------*/
void RTC_Alarm_Handler(char alarm);
void RTC_WakeUp_Handler(void);
void SetRTCAlarm(uint32_t alarmVal);




#ifdef __cplusplus
}
#endif
#endif /*__RTC_H */
