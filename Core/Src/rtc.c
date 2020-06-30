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

/**
  * @brief  RTC Alarm Handler called from interrupt via flags
  * @param  alarm: alarm A or B
  * @retval None
  */
void RTC_Alarm_Handler(char alarm) {

  if (alarm == 'A') {
    LED_Blink(GPIOG, GPIO_BSRR_BS14_Pos);
  }

  if (alarm == 'B') {
    static char dateStr[11];
    static char timeStr[11];
    // // uint32_t tm = LL_RTC_TIME_Get(RTC);
    // // uint32_t dt = LL_RTC_DATE_Get(RTC);
    
    sprintf(timeStr, "%2lx:%02lx:%02lx", LL_RTC_TIME_GetHour(RTC), LL_RTC_TIME_GetMinute(RTC), LL_RTC_TIME_GetSecond(RTC));
    sprintf(dateStr, "%2ld/%ld/%4ld", LL_RTC_DATE_GetDay(RTC), LL_RTC_DATE_GetMonth(RTC), (LL_RTC_DATE_GetYear(RTC) + 1900));
    // printf("date: %s\n\r", dateStr);
    // printf("time: %s\n\r", timeStr);

    PrintDate(L1, dateStr);
    PrintTime(L1, timeStr);
  }
}




/**
  * @brief  RTC WakeUp Handler called from interrupt via flags
  * @param  None:
  * @retval None
  */
void RTC_WakeUp_Handler(void) {
  //
}




/*
 * RTC Alarm Handler called from interrupt via flags
 */
/**
  * @brief  Set RTC Alarm value
  * @param  alarmVal: an alarm value
  * @retval None
  */
void SetRTCAlarm(uint32_t alarmVal) {
  //
}

