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



/* -------------------------------------------------------------------------- */
/* Private function prototypes -----------------------------------------------*/
/* -------------------------------------------------------------------------- */
static void SetupHardware(void);
static void CronSysQuantum_Handler(void);
static void CronMillis_Handler(void);
static void CronSeconds_Handler(void);
static void CronMinutes_Handler(void);
static void Flags_Handler(void);











////////////////////////////////////////////////////////////////////////////////

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
}

// ---- Seconds ---- //
static void CronSeconds_Handler(void) {
  //
  LL_IWDG_ReloadCounter(IWDG);
  LED_Blink(GPIOG, GPIO_BSRR_BS13_Pos);
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

  if (FLAG_CHECK(_EREG_, _EWUPF_)) {
    if (WakeUp_Handler()) {
      FLAG_CLR(_EREG_, _EWUPF_);
    }
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

  if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5) {
    Error_Handler();
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_PWR_EnableOverDriveMode();
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while (!(LL_RCC_HSE_IsReady()));

  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while (!(LL_RCC_LSI_IsReady()));

  LL_PWR_EnableBkUpAccess();
  LL_RCC_ForceBackupDomainReset();
  LL_RCC_ReleaseBackupDomainReset();
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  LL_RCC_EnableRTC();

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 180, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

  /* Wait till PLL is ready */
  while (!(LL_RCC_PLL_IsReady()));

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);
  
  LL_SetSystemCoreClock(180000000);
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);

  LL_RCC_PLLSAI_ConfigDomain_LTDC(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLSAIM_DIV_4, 60, LL_RCC_PLLSAIR_DIV_5, LL_RCC_PLLSAIDIVR_DIV_4);
  LL_RCC_PLLSAI_Enable();

 /* Wait till PLLSAI is ready */
  while (!(LL_RCC_PLLSAI_IsReady()));
  


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
    | LL_AHB1_GRP1_PERIPH_DMA2D
  );

  LL_APB1_GRP1_EnableClock(
      LL_APB1_GRP1_PERIPH_I2C3
    | LL_APB1_GRP1_PERIPH_TIM7
  );

  LL_APB2_GRP1_EnableClock(
      LL_APB2_GRP1_PERIPH_SPI5
    | LL_APB2_GRP1_PERIPH_USART1
    | LL_APB2_GRP1_PERIPH_LTDC
  );

  LL_AHB3_GRP1_EnableClock(
    LL_AHB3_GRP1_PERIPH_FMC
  );
  while (!(GET_PERIPH_BB_VAL((uint32_t)RCC, RCC_AHB3_Offset, RCC_AHB3ENR_FMCEN_Pos)));



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
  NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
  NVIC_EnableIRQ(EXTI0_IRQn);

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
  GPIO_InitStruct.Speed                 = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType            = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull                  = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate             = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*
  PF6   ------> SPI5_NSS
  */ 
  // GPIO_InitStruct.Pin                   = LL_GPIO_PIN_6;
  // GPIO_InitStruct.Mode                  = LL_GPIO_MODE_OUTPUT;
  // GPIO_InitStruct.Speed                 = LL_GPIO_SPEED_FREQ_LOW;
  // GPIO_InitStruct.OutputType            = LL_GPIO_OUTPUT_PUSHPULL;
  // GPIO_InitStruct.Pull                  = LL_GPIO_PULL_NO;
  // LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* SPI5 parameter configuration*/
  SPI_InitStruct.TransferDirection      = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode                   = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth              = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity          = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase             = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS                    = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate               = LL_SPI_BAUDRATEPRESCALER_DIV16;
  SPI_InitStruct.BitOrder               = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation         = LL_SPI_CRCCALCULATION_DISABLE;
  // SPI_InitStruct.CRCPoly                = 7;
  LL_SPI_Init(SPI5, &SPI_InitStruct);
  // LL_SPI_SetStandard(SPI5, LL_SPI_PROTOCOL_MOTOROLA);

  LL_SPI_Enable(SPI5);







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
    RTC_TimeStruct.Hours                  = 12;
    RTC_TimeStruct.Minutes                = 0;
    RTC_TimeStruct.Seconds                = 0;
    LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_TimeStruct);

    RTC_DateStruct.WeekDay                = LL_RTC_WEEKDAY_THURSDAY;
    RTC_DateStruct.Day                    = 5;
    RTC_DateStruct.Month                  = LL_RTC_MONTH_MARCH;
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


/************************************************************************************************/
// FMC
  FMC_SDRAM_InitTypeDef Sdram_InitStruct  = {0};
  FMC_SDRAM_TimingTypeDef SdramTiming     = {0};

  /** FMC GPIO Configuration  
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PC0   ------> FMC_SDNWE
  PF11  ------> FMC_SDNRAS
  PF12  ------> FMC_A6
  PF13  ------> FMC_A7
  PF14  ------> FMC_A8
  PF15  ------> FMC_A9
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10  ------> FMC_D7
  PE11  ------> FMC_D8
  PE12  ------> FMC_D9
  PE13  ------> FMC_D10
  PE14  ------> FMC_D11
  PE15  ------> FMC_D12
  PD8   ------> FMC_D13
  PD9   ------> FMC_D14
  PD10  ------> FMC_D15
  PD14  ------> FMC_D0
  PD15  ------> FMC_D1
  PG4   ------> FMC_BA0
  PG5   ------> FMC_BA1
  PG8   ------> FMC_SDCLK
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PG15  ------> FMC_SDNCAS
  PB5   ------> FMC_SDCKE1
  PB6   ------> FMC_SDNE1
  PE0   ------> FMC_NBL0
  PE1   ------> FMC_NBL1
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode                    = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed                   = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType              = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull                    = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate               = LL_GPIO_AF_12;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                     = LL_GPIO_PIN_0;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_8 | LL_GPIO_PIN_15;
  LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_7 | LL_GPIO_PIN_8 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15 | LL_GPIO_PIN_0 | LL_GPIO_PIN_1;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_8 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15 | LL_GPIO_PIN_0 | LL_GPIO_PIN_1;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                     = LL_GPIO_PIN_5 | LL_GPIO_PIN_6;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  Sdram_InitStruct.SDBank                 = FMC_SDRAM_BANK2;
  Sdram_InitStruct.ColumnBitsNumber       = FMC_SDRAM_COLUMN_BITS_NUM_8;
  Sdram_InitStruct.RowBitsNumber          = FMC_SDRAM_ROW_BITS_NUM_12;
  Sdram_InitStruct.MemoryDataWidth        = FMC_SDRAM_MEM_BUS_WIDTH_16;
  Sdram_InitStruct.InternalBankNumber     = FMC_SDRAM_INTERN_BANKS_NUM_4;
  Sdram_InitStruct.CASLatency             = FMC_SDRAM_CAS_LATENCY_3;
  Sdram_InitStruct.WriteProtection        = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  Sdram_InitStruct.SDClockPeriod          = FMC_SDRAM_CLOCK_PERIOD_2;
  Sdram_InitStruct.ReadBurst              = FMC_SDRAM_RBURST_DISABLE;
  Sdram_InitStruct.ReadPipeDelay          = FMC_SDRAM_RPIPE_DELAY_0;
  SDRAM_Init(FMC_SDRAM_DEVICE, &Sdram_InitStruct);

  SdramTiming.LoadToActiveDelay           = 2;
  SdramTiming.ExitSelfRefreshDelay        = 7;
  SdramTiming.SelfRefreshTime             = 4;
  SdramTiming.RowCycleDelay               = 7;
  SdramTiming.WriteRecoveryTime           = 2;
  SdramTiming.RPDelay                     = 2;
  SdramTiming.RCDDelay                    = 2;
  SDRAM_Timing_Init(FMC_SDRAM_DEVICE, &SdramTiming, Sdram_InitStruct.SDBank);

  FMC_SDRAM_CommandTypeDef SdramCommand_InitStruct  = {0};

  SdramCommand_InitStruct.CommandMode               = FMC_SDRAM_CMD_CLK_ENABLE;
  SdramCommand_InitStruct.CommandTarget             = FMC_SDRAM_CMD_TARGET_BANK2;
  SdramCommand_InitStruct.AutoRefreshNumber         = 1;
  SdramCommand_InitStruct.ModeRegisterDefinition    = 0;
  SDRAM_SendCommand(FMC_SDRAM_DEVICE, &SdramCommand_InitStruct);
  Delay(10);

  SdramCommand_InitStruct.CommandMode               = FMC_SDRAM_CMD_PALL;
  SDRAM_SendCommand(FMC_SDRAM_DEVICE, &SdramCommand_InitStruct);

  SdramCommand_InitStruct.CommandMode               = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  SdramCommand_InitStruct.AutoRefreshNumber         = 4;
  SDRAM_SendCommand(FMC_SDRAM_DEVICE, &SdramCommand_InitStruct);
  SDRAM_SendCommand(FMC_SDRAM_DEVICE, &SdramCommand_InitStruct);

  uint32_t tmpr = (uint32_t)(
      SDRAM_MODEREG_BURST_LENGTH_2
    | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL
    | SDRAM_MODEREG_CAS_LATENCY_3
    | SDRAM_MODEREG_OPERATING_MODE_STANDARD
    | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE
  );

  SdramCommand_InitStruct.CommandMode               = FMC_SDRAM_CMD_LOAD_MODE;
  SdramCommand_InitStruct.AutoRefreshNumber         = 1;
  SdramCommand_InitStruct.ModeRegisterDefinition    = tmpr;
  SDRAM_SendCommand(FMC_SDRAM_DEVICE, &SdramCommand_InitStruct);

  SDRAM_ProgramRefreshRate(FMC_SDRAM_DEVICE, 683);





/************************************************************************************************/
// DMA2
  /* DMA2D interrupt Init */
  // NVIC_SetPriority(DMA2D_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
  // NVIC_EnableIRQ(DMA2D_IRQn);

  // LL_DMA2D_SetMode(DMA2D, LL_DMA2D_MODE_M2M);
  // LL_DMA2D_SetOutputColorMode(DMA2D, LL_DMA2D_OUTPUT_MODE_RGB565);
  // LL_DMA2D_SetLineOffset(DMA2D, 0);
  // LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_RGB565);
  // LL_DMA2D_FGND_SetAlphaMode(DMA2D, LL_DMA2D_ALPHA_MODE_NO_MODIF);
  // LL_DMA2D_FGND_SetAlpha(DMA2D, 0);
  // LL_DMA2D_FGND_SetLineOffset(DMA2D, 0);





/************************************************************************************************/
// LTDC
  /**LTDC GPIO Configuration    
  PF10    ------> LTDC_DE
  PA3     ------> LTDC_B5
  PA4     ------> LTDC_VSYNC
  PA6     ------> LTDC_G2
  PB0     ------> LTDC_R3
  PB1     ------> LTDC_R6
  PB10    ------> LTDC_G4
  PB11    ------> LTDC_G5
  PG6     ------> LTDC_R7
  PG7     ------> LTDC_CLK
  PC6     ------> LTDC_HSYNC
  PC7     ------> LTDC_G6
  PA11    ------> LTDC_R4
  PA12    ------> LTDC_R5
  PC10    ------> LTDC_R2
  PD3     ------> LTDC_G7
  PD6     ------> LTDC_B2
  PG10    ------> LTDC_G3
  PG11    ------> LTDC_B3
  PG12    ------> LTDC_B4
  PB8     ------> LTDC_B6
  PB9     ------> LTDC_B7
  PD11    ------> TE
  PD12    ------> RDX
  PD13    ------> WRX
  PC2     ------> SPI_CS
  */
  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode                   = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed                  = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType             = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull                   = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate              = LL_GPIO_AF_14;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);


  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_6 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_8 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_11;
  LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_10;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_3 | LL_GPIO_PIN_6;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_0 | LL_GPIO_PIN_1;
  GPIO_InitStruct.Alternate             = LL_GPIO_AF_9;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_10 | LL_GPIO_PIN_12;
  LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode                  = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed                 = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull                  = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_12 | LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode                  = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed                 = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType            = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull                  = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin                   = LL_GPIO_PIN_2;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  LCD_CS_H;
  LCD_WRX_L;
  ILI9341_Init();


  LTDC_InitTypeDef LTDC_InitStruct                = {0};
  LTDC_LayerCfgTypeDef LTDC_Layer_InitStruct      = {0};

  LTDC_InitStruct.HSPolarity                      = LTDC_HSPOLARITY_AL;
  LTDC_InitStruct.VSPolarity                      = LTDC_VSPOLARITY_AL;
  LTDC_InitStruct.DEPolarity                      = LTDC_DEPOLARITY_AL;
  LTDC_InitStruct.PCPolarity                      = LTDC_PCPOLARITY_IIPC;
  LTDC_InitStruct.HorizontalSync                  = 9;
  LTDC_InitStruct.VerticalSync                    = 1;
  LTDC_InitStruct.AccumulatedHBP                  = 29;
  LTDC_InitStruct.AccumulatedVBP                  = 3;
  LTDC_InitStruct.AccumulatedActiveW              = ILI9341_HEIGHT + 20 + LTDC_InitStruct.AccumulatedHBP; // 240 + LTDC_InitStruct.AccumulatedHBP;
  LTDC_InitStruct.AccumulatedActiveH              = ILI9341_WIDTH + 20 + LTDC_InitStruct.AccumulatedVBP; // 320 + LTDC_InitStruct.AccumulatedVBP;
  LTDC_InitStruct.TotalWidth                      = LTDC_InitStruct.AccumulatedActiveW + 10;
  LTDC_InitStruct.TotalHeigh                      = LTDC_InitStruct.AccumulatedActiveH + 4 ;

  LTDC_InitStruct.Backcolor.Red                   = (uint8_t)(ARGB8888_Red >> 16);
  LTDC_InitStruct.Backcolor.Green                 = (uint8_t)(ARGB8888_Red >> 8);
  LTDC_InitStruct.Backcolor.Blue                  = (uint8_t)ARGB8888_Red;
  LTDC_Init(LTDC, &LTDC_InitStruct);

  LTDC_Layer_InitStruct.WindowX0                  = 10;
  LTDC_Layer_InitStruct.WindowX1                  = ILI9341_HEIGHT + 10; //230;
  LTDC_Layer_InitStruct.WindowY0                  = 10;
  LTDC_Layer_InitStruct.WindowY1                  = ILI9341_WIDTH + 10; //310;
  LTDC_Layer_InitStruct.PixelFormat               = LTDC_PIXEL_FORMAT_ARGB8888;
  LTDC_Layer_InitStruct.Alpha                     = 255;
  LTDC_Layer_InitStruct.BlendingFactor1           = LTDC_BLENDING_FACTOR1_CA;
  LTDC_Layer_InitStruct.BlendingFactor2           = LTDC_BLENDING_FACTOR2_CA;
  LTDC_Layer_InitStruct.FBStartAdress             = ILI9341_L1_ADDR;
  LTDC_Layer_InitStruct.ImageWidth                = ILI9341_HEIGHT; //220;
  LTDC_Layer_InitStruct.ImageHeight               = ILI9341_WIDTH; //300;
  LTDC_Layer_InitStruct.Alpha0                    = 0;
  LTDC_Layer_InitStruct.Backcolor.Red             = (uint8_t)(ARGB8888_Apple >> 16);
  LTDC_Layer_InitStruct.Backcolor.Green           = (uint8_t)(ARGB8888_Apple >> 8);
  LTDC_Layer_InitStruct.Backcolor.Blue            = (uint8_t)ARGB8888_Apple;
  LTDC_SetConfig(LTDC, &LTDC_Layer_InitStruct, L1);

  // LTDC_Layer_InitStruct.WindowX0                  = 0;
  // LTDC_Layer_InitStruct.WindowX1                  = 0;
  // LTDC_Layer_InitStruct.WindowY0                  = 0;
  // LTDC_Layer_InitStruct.WindowY1                  = 0;
  // LTDC_Layer_InitStruct.PixelFormat               = LTDC_PIXEL_FORMAT_ARGB8888;
  LTDC_Layer_InitStruct.Alpha                     = 255;
  LTDC_Layer_InitStruct.BlendingFactor1           = LTDC_BLENDING_FACTOR1_PAxCA;
  LTDC_Layer_InitStruct.BlendingFactor2           = LTDC_BLENDING_FACTOR2_PAxCA;
  LTDC_Layer_InitStruct.FBStartAdress             = ILI9341_L2_ADDR;
  // LTDC_Layer_InitStruct.ImageWidth                = 0;
  // LTDC_Layer_InitStruct.ImageHeight               = 0;
  LTDC_Layer_InitStruct.Alpha0                    = 255;
  LTDC_Layer_InitStruct.Backcolor.Red             = (uint8_t)(ARGB8888_Orange >> 16);
  LTDC_Layer_InitStruct.Backcolor.Green           = (uint8_t)(ARGB8888_Orange >> 8);
  LTDC_Layer_InitStruct.Backcolor.Blue            = (uint8_t)ARGB8888_Orange;
  // LTDC_SetConfig(LTDC, &LTDC_Layer_InitStruct, L2);

  LTDC_Enable(LTDC);

  FillLayer(L1, ARGB8888_Darkgreen);






/************************************************************************************************/
// Display
  DrawPixel(L1, 20, 10, ARGB8888_Red);

  DrawVLine(L1, 20, 20, 180, ARGB8888_White);
  DrawHLine(L1, 20, 20, 180, ARGB8888_White);

  // FillRectangle(L1, 40, 60, 100, 50, ARGB8888_White);
}











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
