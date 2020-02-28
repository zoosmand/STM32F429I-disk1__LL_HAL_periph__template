/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Global variables ---------------------------------------------------------*/
uint32_t sysQuantum = 0;
uint32_t millis     = 0;
uint32_t seconds    = 0;
uint32_t minutes    = 0;
uint32_t _EREG_     = 0;


/* Private variables ---------------------------------------------------------*/
static uint32_t millis_tmp    = 100;
static uint32_t seconds_tmp   = 1000;
static uint32_t minutes_tmp   = 60;

static uint32_t dess = 0;
static uint32_t dess2 = 0;
static uint32_t dess3 = 0;



/* -------------------------------------------------------------------------- */
/* Private function prototypes -----------------------------------------------*/
/* -------------------------------------------------------------------------- */
static void SetupHardware(void);
static void CronSysQuantum_Handler(void);
static void CronMillis_Handler(void);
static void CronSeconds_Handler(void);
static void CronMinutes_Handler(void);
static void Flags_Handler(void);













/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {

  SetupHardware();

  while (1) {
    Delay_Handler(0);
    Cron_Handler();
    Flags_Handler();
  }
}



/********************************************************************************/
/*                                     CRON                                     */
/********************************************************************************/
void Cron_Handler() {
  $CronStart:
  if (SysTick->CTRL & (1 << SysTick_CTRL_COUNTFLAG_Pos)) { 
    sysQuantum++;
    CronSysQuantum_Handler();
  }

  if (sysQuantum >= millis_tmp) {
    millis++;
    millis_tmp = sysQuantum + 100;
    CronMillis_Handler();
  }
  
  if (millis >= seconds_tmp) {
    seconds++;
    seconds_tmp += 1000;
    CronSeconds_Handler();
  }
  
  if (seconds >= minutes_tmp) {
    minutes++;
    minutes_tmp += 60;
    CronMinutes_Handler();
  }

  while (sysQuantum < delay_tmp) {
    goto $CronStart;
  }
  // !!!!!!!!! The bug!!!!!!!!
  delay_tmp = 0;
  FLAG_CLR(_EREG_, _DELAYF_);
}



/********************************************************************************/
/*                             CRON EVENTS HANDLERS                             */
/********************************************************************************/
// ---- System Quantum ---- //
static void CronSysQuantum_Handler(void) {
  //
}

// ---- Milliseconds ---- //
static void CronMillis_Handler(void) {
  //
  dess++;
  dess3 +=3;
}

// ---- Seconds ---- //
static void CronSeconds_Handler(void) {
  //
  LL_IWDG_ReloadCounter(IWDG);
  LED_Blink(GPIOG, GPIO_BSRR_BS13_Pos);
  // printf("Helllo!\n");
}

// ---- Minutes ---- //
static void CronMinutes_Handler(void) {
  //
}



/********************************************************************************/
/*                                     FLAGS                                    */
/********************************************************************************/
void Flags_Handler(void){
  if (FLAG_CHECK(_EREG_, _BT7F_)) {
    BasicTimer_FromIT_Handler(TIM7);
    FLAG_CLR(_EREG_, _BT7F_);
  }

  if (FLAG_CHECK(_EREG_, _RTCALAF_)) {
    RTC_Alarm_Handler('A');
    FLAG_CLR(_EREG_, _RTCALAF_);
  }

  if (FLAG_CHECK(_EREG_, _RTCALBF_)) {
    RTC_Alarm_Handler('B');
    FLAG_CLR(_EREG_, _RTCALBF_);
  }

  if (FLAG_CHECK(_EREG_, _RTWUPF_)) {
    RTC_WakeUp_Handler();
    FLAG_CLR(_EREG_, _RTWUPF_);
  }

  if (FLAG_CHECK(_EREG_, _RDF_)) {
    //Display_Handler(TM1637);
    FLAG_CLR(_EREG_, _RDF_);
  }

  // ---- USART Registry ----
  // if (FLAG_CHECK(_USARTREG_, _RXITF_)) {
  //   USART1_RX_Handler();
  //   FLAG_CLR(_USARTREG_, _RXITF_);
  // }
  
  if (FLAG_CHECK(_EREG_, _BLINKF_)) {
    FLAG_CLR(_EREG_, _BLINKF_);
  }
}







/********************************************************************************/
/*                             INITIALIZATION BLOCK                             */
/********************************************************************************/
/**
  * @brief Hardware configuration
  * @retval None
  */
static void SetupHardware(void) {
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* Debug frozes */
  LL_DBGMCU_APB1_GRP1_FreezePeriph(
      LL_DBGMCU_APB1_GRP1_TIM7_STOP
    | LL_DBGMCU_APB1_GRP1_IWDG_STOP
    | LL_DBGMCU_APB1_GRP1_WWDG_STOP
  );

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* RCC interrupt configuration */
  NVIC_SetPriority(RCC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(RCC_IRQn);

  /* SysTick */
  SysTick->CTRL     = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
  SysTick->LOAD     = 1800U - 1U;
  SysTick->VAL      = 0;
  SysTick->CTRL     |= SysTick_CTRL_ENABLE_Msk;

  /* SysTick interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(SysTick_IRQn);

  /* System Clock Configuration */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5) {
    Error_Handler();
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_PWR_EnableOverDriveMode();
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(!(LL_RCC_HSE_IsReady()));

  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(!(LL_RCC_LSI_IsReady()));

  LL_PWR_EnableBkUpAccess();
  LL_RCC_ForceBackupDomainReset();
  LL_RCC_ReleaseBackupDomainReset();
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  LL_RCC_EnableRTC();
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 180, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(!(LL_RCC_PLL_IsReady()));

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);
  
  LL_SetSystemCoreClock(180000000);
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);




/************************************************************************************************/
// IWDG Initialization Function
  LL_IWDG_Enable(IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_128);
  LL_IWDG_SetReloadCounter(IWDG, 6625);
  while (!(LL_IWDG_IsReady(IWDG)));

  LL_IWDG_ReloadCounter(IWDG);






/************************************************************************************************/
// Peripheral clock

  LL_AHB1_GRP1_EnableClock(
      LL_AHB1_GRP1_PERIPH_GPIOA
    | LL_AHB1_GRP1_PERIPH_GPIOB
    | LL_AHB1_GRP1_PERIPH_GPIOC
    | LL_AHB1_GRP1_PERIPH_GPIOD
    | LL_AHB1_GRP1_PERIPH_GPIOE
    | LL_AHB1_GRP1_PERIPH_GPIOF
    | LL_AHB1_GRP1_PERIPH_GPIOG
    | LL_AHB1_GRP1_PERIPH_GPIOH
    | LL_AHB1_GRP1_PERIPH_CRC
  );

  LL_APB1_GRP1_EnableClock(
      LL_APB1_GRP1_PERIPH_I2C3
    | LL_APB1_GRP1_PERIPH_TIM7
  );

  LL_APB2_GRP1_EnableClock(
      LL_APB2_GRP1_PERIPH_SPI5
    | LL_APB2_GRP1_PERIPH_USART1
  );




/************************************************************************************************/
// GPIO LED
  LL_GPIO_InitTypeDef GPIO_InitStruct   = {0};
  LL_EXTI_InitTypeDef EXTI_InitStruct   = {0};
  
  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_13 | LL_GPIO_PIN_14;
  GPIO_InitStruct.Mode                  = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed                 = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType            = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull                  = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

// GPIO Wake-Up PA0
  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode                  = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull                  = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* RTC interrupt Init */
  NVIC_SetPriority(RTC_Alarm_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 14, 0));
  NVIC_EnableIRQ(RTC_Alarm_IRQn);

  /* EXTI channel to catch an interrupt */
  EXTI_InitStruct.Line_0_31             = LL_EXTI_LINE_0;
  EXTI_InitStruct.Mode                  = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger               = LL_EXTI_TRIGGER_RISING;
  EXTI_InitStruct.LineCommand           = ENABLE;
  LL_EXTI_Init(&EXTI_InitStruct);




/************************************************************************************************/
// SPI 5
  LL_SPI_InitTypeDef SPI_InitStruct     = {0};
  /**SPI5 GPIO Configuration  
  PF7   ------> SPI5_SCK
  PF8   ------> SPI5_MISO
  PF9   ------> SPI5_MOSI 
  */
  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_7 | LL_GPIO_PIN_8 | LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode                  = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed                 = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType            = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull                  = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate             = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*
  PF6   ------> SPI5_NSS
  */ 
  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode                  = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed                 = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType            = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull                  = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* SPI5 parameter configuration*/
  SPI_InitStruct.TransferDirection      = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode                   = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth              = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity          = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase             = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS                    = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate               = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI_InitStruct.BitOrder               = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation         = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly                = 10;
  LL_SPI_Init(SPI5, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI5, LL_SPI_PROTOCOL_MOTOROLA);








/************************************************************************************************/
// I2C 3
  LL_I2C_InitTypeDef I2C_InitStruct     = {0};

  /**I2C3 GPIO Configuration  
  PC9   ------> I2C3_SDA
  PA8   ------> I2C3_SCL 
  */
  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode                  = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed                 = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType            = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull                  = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate             = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode                  = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed                 = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType            = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull                  = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate             = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  LL_I2C_DisableOwnAddress2(I2C3);
  LL_I2C_DisableGeneralCall(I2C3);
  LL_I2C_EnableClockStretching(I2C3);
  I2C_InitStruct.PeripheralMode         = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed             = 100000;
  I2C_InitStruct.DutyCycle              = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.AnalogFilter           = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter          = 0;
  I2C_InitStruct.OwnAddress1            = 0;
  I2C_InitStruct.TypeAcknowledge        = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize            = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C3, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C3, 0);







/************************************************************************************************/
// USART 1
  LL_USART_InitTypeDef USART_InitStruct   = {0};
  /**USART1 GPIO Configuration  
  PA9   ------> USART1_TX
  PA10  ------> USART1_RX 
  */
  GPIO_InitStruct.Pin                     = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode                    = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed                   = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType              = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull                    = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate               = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  USART_InitStruct.BaudRate               = 115200;
  USART_InitStruct.DataWidth              = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits               = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity                 = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection      = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl    = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling           = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);









/************************************************************************************************/
// TIMER 7
  LL_TIM_InitTypeDef TIM_InitStruct       = {0};

  /* TIM7 interrupt Init */
  NVIC_SetPriority(TIM7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
  NVIC_EnableIRQ(TIM7_IRQn);

  TIM_InitStruct.Prescaler                = 9000U - 1U; // APB1 runs on max freq of SystemCoreClock/2 (here 90 MHz)
  TIM_InitStruct.CounterMode              = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload               = 10000U - 1U;
  LL_TIM_Init(TIM7, &TIM_InitStruct);
  LL_TIM_EnableARRPreload(TIM7);

  LL_TIM_ClearFlag_UPDATE(TIM7);
  LL_TIM_EnableIT_UPDATE(TIM7);
  LL_TIM_EnableCounter(TIM7);
  // LL_TIM_SetTriggerOutput(TIM7, LL_TIM_TRGO_ENABLE);
  // LL_TIM_DisableMasterSlaveMode(TIM7);









/************************************************************************************************/
// RTC
  LL_RTC_InitTypeDef RTC_InitStruct       = {0};
  LL_RTC_TimeTypeDef RTC_TimeStruct       = {0};
  LL_RTC_DateTypeDef RTC_DateStruct       = {0};
  LL_RTC_AlarmTypeDef RTC_AlarmStruct     = {0};

  /* Peripheral clock enable */
  LL_RCC_EnableRTC();

  RTC_InitStruct.HourFormat               = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler          = 127;
  RTC_InitStruct.SynchPrescaler           = 255;
  LL_RTC_Init(RTC, &RTC_InitStruct);
  LL_RTC_SetAsynchPrescaler(RTC, 127);
  LL_RTC_SetSynchPrescaler(RTC, 255);

  if(LL_RTC_BAK_GetRegister(RTC, LL_RTC_BKP_DR0) != 0x32F2) {
    RTC_TimeStruct.Hours                  = 18;
    RTC_TimeStruct.Minutes                = 0;
    RTC_TimeStruct.Seconds                = 0;
    LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_TimeStruct);

    RTC_DateStruct.WeekDay                = LL_RTC_WEEKDAY_THURSDAY;
    RTC_DateStruct.Day                    = 27;
    RTC_DateStruct.Month                  = LL_RTC_MONTH_FEBRUARY;
    RTC_DateStruct.Year                   = 120;
    LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_DateStruct);

    LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR0, 0x32f2);
  }
  
  /* Enable Alarm A */
  RTC_AlarmStruct.AlarmTime.Hours         = 0x0;
  RTC_AlarmStruct.AlarmTime.Minutes       = 0x0;
  RTC_AlarmStruct.AlarmTime.Seconds       = 0x0;
  RTC_AlarmStruct.AlarmMask               = LL_RTC_ALMA_MASK_ALL;
  RTC_AlarmStruct.AlarmDateWeekDaySel     = LL_RTC_ALMA_DATEWEEKDAYSEL_DATE;
  RTC_AlarmStruct.AlarmDateWeekDay        = 1;
  LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_AlarmStruct);
  LL_RTC_EnableIT_ALRA(RTC);

  /* Enable Alarm B */
  RTC_AlarmStruct.AlarmMask               = LL_RTC_ALMB_MASK_ALL;
  RTC_AlarmStruct.AlarmDateWeekDaySel     = LL_RTC_ALMB_DATEWEEKDAYSEL_DATE;
  RTC_AlarmStruct.AlarmDateWeekDay        = 1;
  LL_RTC_ALMB_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_AlarmStruct);
  LL_RTC_EnableIT_ALRB(RTC);

  /* Set alarms */
  LL_RTC_DisableWriteProtection(RTC);
  /* ~0.5s */
  RTC->ALRMASSR = (6 << RTC_ALRMASSR_MASKSS_Pos) | (0x0000 << RTC_ALRMASSR_SS_Pos);
  /* ~1s */
  RTC->ALRMBSSR = (8 << RTC_ALRMBSSR_MASKSS_Pos) | (0x0000 << RTC_ALRMBSSR_SS_Pos);

  LL_RTC_ClearFlag_ALRA(RTC);
  LL_RTC_EnableIT_ALRA(RTC);
  LL_RTC_ALMA_Enable(RTC);

  LL_RTC_ClearFlag_ALRB(RTC);
  LL_RTC_EnableIT_ALRB(RTC);
  LL_RTC_ALMB_Enable(RTC);

  LL_RTC_EnableWriteProtection(RTC);

  /* RTC interrupt Init */
  NVIC_SetPriority(RTC_Alarm_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 14, 0));
  NVIC_EnableIRQ(RTC_Alarm_IRQn);

  /* EXTI channel to catch an interrupt */
  EXTI_InitStruct.Line_0_31               = LL_EXTI_LINE_17;
  EXTI_InitStruct.Mode                    = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger                 = LL_EXTI_TRIGGER_RISING;
  EXTI_InitStruct.LineCommand             = ENABLE;
  LL_EXTI_Init(&EXTI_InitStruct);

}







/* FMC initialization function */
// static void MX_FMC_Init(void) {
//   FMC_SDRAM_TimingTypeDef SdramTiming = {0};

//   hsdram1.Instance = FMC_SDRAM_DEVICE;
//   /* hsdram1.Init */
//   hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
//   hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
//   hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
//   hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
//   hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
//   hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_1;
//   hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
//   hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_DISABLE;
//   hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
//   hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
//   /* SdramTiming */
//   SdramTiming.LoadToActiveDelay = 16;
//   SdramTiming.ExitSelfRefreshDelay = 16;
//   SdramTiming.SelfRefreshTime = 16;
//   SdramTiming.RowCycleDelay = 16;
//   SdramTiming.WriteRecoveryTime = 16;
//   SdramTiming.RPDelay = 16;
//   SdramTiming.RCDDelay = 16;

//   if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK);
// }




















#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) { 
     printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
