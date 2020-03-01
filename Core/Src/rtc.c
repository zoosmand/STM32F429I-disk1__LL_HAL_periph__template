/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : 
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"









////////////////////////////////////////////////////////////////////////////////

/*
 * RTC Alarm Handler called from interrupt via flags
 */
void RTC_Alarm_Handler(char alarm) {

  if (alarm == 'A') {
    LED_Blink(GPIOG, GPIO_BSRR_BS14_Pos);
  }

  if (alarm == 'B') {
    static char dateStr[11];
    static char timeStr[11];
    // uint32_t tm = LL_RTC_TIME_Get(RTC);
    // uint32_t dt = LL_RTC_DATE_Get(RTC);
    
    // sprintf(timeStr, "%2ld:%02ld:%02ld", LL_RTC_TIME_GetHour(RTC), LL_RTC_TIME_GetMinute(RTC), LL_RTC_TIME_GetSecond(RTC));
    // sprintf(dateStr, "%2ld/%02ld/%4ld", LL_RTC_DATE_GetDay(RTC), LL_RTC_DATE_GetMonth(RTC), (LL_RTC_DATE_GetYear(RTC) + 1900));
    // printf("date: %s\n\r", dateStr);
    // printf("time: %s\n\r", timeStr);
  }
}



/*
 * RTC WakeUp Handler called from interrupt via flags
 */
void RTC_WakeUp_Handler(void) {
  //
}



/*
 * RTC WakeUp Handler called from interrupt via flags
 */
void SetRTCAlarm(uint32_t alarmVal) {
  //
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
