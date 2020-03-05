/* Includes ------------------------------------------------------------------*/
#include "sdram.h"









////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Write a byte into SDRAM's certain address
  * @param  addr: SDRAM address
  * @param  data: a byte   
  * @retval none
  */
void SDRAM_Write8b(uint32_t addr, uint8_t data) {
  *(__IO uint8_t*) (SDRAM_START_ADDR + addr) = data;
}




/**
  * @brief  Read a byte from SDRAM's certain address
  * @param  addr: SDRAM address
  * @retval a byte
  */
uint8_t SDRAM_Read8b(uint32_t addr) {
  uint8_t data = 0;
  data = *(__IO uint8_t*)(SDRAM_START_ADDR + addr);
  return (data);
}




/**
  * @brief  Write a halfword into SDRAM's certain address
  * @param  addr: SDRAM address
  * @param  data: a halfword
  * @retval none
  */
void SDRAM_Write16b(uint32_t addr, uint16_t data) {
  *(__IO uint16_t*) (SDRAM_START_ADDR + addr) = data;
}




/**
  * @brief  Read a halfword from SDRAM's certain address
  * @param  addr: SDRAM address
  * @retval a halfword
  */
uint16_t SDRAM_Read16b(uint32_t addr) {
  uint16_t data = 0;
  data = *(__IO uint16_t*)(SDRAM_START_ADDR + addr);
  return (data);
}




/**
  * @brief  Write a word into SDRAM's certain address
  * @param  addr: SDRAM address
  * @param  data: a word
  * @retval none
  */
void SDRAM_Write32b(uint32_t addr, uint32_t data) {
  *(__IO uint32_t*)(SDRAM_START_ADDR + addr) = data;
}




/**
  * @brief  Read a word from SDRAM's certain address
  * @param  addr SDRAM address
  * @retval a word
  */
uint32_t SDRAM_Read32b(uint32_t addr) {
  uint32_t data = 0;
  data = *(__IO uint32_t*)(SDRAM_START_ADDR + addr);
  return (data);
}




/**
  * @brief  Read a buffer from SDRAM's certain address
  * @param  addr SDRAM address
  * @param  cnt count of bytes
  * @param  buf pointer of a buffer
  * @retval status of reading
  */
uint8_t SDRAM_ReadBuffer8b(uint32_t addr, uint32_t cnt, uint8_t *buf) {
  if ((addr + cnt) > SDRAM_MAX_ADDR) return (0);
  uint32_t ptr = addr;

  while (SDRAM_BUSY_STATUS_CHECK);

  while (cnt--) {
    *buf++ = *(__IO uint8_t*)(SDRAM_START_ADDR + ptr);
    ptr++;
  }
  return (1);
}




/**
  * @brief  Write a buffer into SDRAM's certain address
  * @param  addr SDRAM address
  * @param  cnt count of bytes
  * @param  buf pointer of a buffer
  * @retval status of writing
  */
uint8_t SDRAM_WriteBuffer8b(uint32_t addr, uint32_t cnt, uint8_t *buf) {
  if ((addr + cnt) > SDRAM_MAX_ADDR) return (0);
  uint32_t ptr = addr;

  while (SDRAM_BUSY_STATUS_CHECK);

  while (cnt--) {
    *(__IO uint8_t*)(SDRAM_START_ADDR + ptr) = *buf++;
    ptr++;
  }
  return (1);
}




/**
  * @brief  Read a buffer from SDRAM's certain address
  * @param  addr SDRAM address
  * @param  cnt count of words
  * @param  buf pointer of a buffer
  * @retval status of reading
  */
uint8_t SDRAM_ReadBuffer(uint32_t addr, uint32_t cnt, uint32_t *buf) {
  if ((addr + cnt) > SDRAM_MAX_ADDR) return (0);
  uint32_t ptr = addr;

  while (SDRAM_BUSY_STATUS_CHECK);

  while (cnt--) {
    *buf++ = *(__IO uint32_t*)(SDRAM_START_ADDR + ptr);
    ptr += 4;
  }
  return (1);
}




/**
  * @brief  Write a buffer into SDRAM's certain address
  * @param  addr SDRAM address
  * @param  cnt count of words
  * @param  buf pointer of a buffer
  * @retval status of writing
  */
uint8_t SDRAM_WriteBuffer(uint32_t addr, uint32_t cnt, uint32_t *buf) {
  if ((addr + cnt) > SDRAM_MAX_ADDR) return (0);
  uint32_t ptr = addr;

  while (SDRAM_BUSY_STATUS_CHECK);

  while (cnt--) {
    *(__IO uint32_t*)(SDRAM_START_ADDR + ptr) = *buf++;
    ptr += 4;
  }
  return (1);
}




/**
  * @brief  Initializes the FMC_SDRAM device according to the specified
  *         control parameters in the FMC_SDRAM_InitTypeDef
  * @param  Device: Pointer to SDRAM device instance
  * @param  Init: Pointer to SDRAM Initialization structure   
  * @retval init status
  */
uint8_t SDRAM_Init(FMC_SDRAM_TypeDef *Device, FMC_SDRAM_InitTypeDef *Init) {
  uint32_t tmpr1 = 0U;
  uint32_t tmpr2 = 0U;
  uint8_t status = 1;
    
  /* Set SDRAM bank configuration parameters */
  if (Init->SDBank != FMC_SDRAM_BANK2) {    
    tmpr1 = Device->SDCR[FMC_SDRAM_BANK1];
    tmpr1 = (uint32_t)0;
    tmpr1 |= (uint32_t)(
        Init->ColumnBitsNumber
      | Init->RowBitsNumber
      | Init->MemoryDataWidth
      | Init->InternalBankNumber
      | Init->CASLatency
      | Init->WriteProtection
      | Init->SDClockPeriod
      | Init->ReadBurst
      | Init->ReadPipeDelay
    );

    Device->SDCR[FMC_SDRAM_BANK1] = tmpr1;
  } else {
    /* FMC_Bank2_SDRAM */ 
    tmpr1 = Device->SDCR[FMC_SDRAM_BANK1];
    tmpr1 = (uint32_t)0;
    tmpr1 |= (uint32_t)(
        Init->SDClockPeriod
      | Init->ReadBurst
      | Init->ReadPipeDelay
    );  
    
    tmpr2 = Device->SDCR[FMC_SDRAM_BANK2];
    tmpr2 = (uint32_t)0;
    tmpr2 |= (uint32_t)(
        Init->ColumnBitsNumber
      | Init->RowBitsNumber
      | Init->MemoryDataWidth
      | Init->InternalBankNumber
      | Init->CASLatency
      | Init->WriteProtection
    );

    Device->SDCR[FMC_SDRAM_BANK1] = tmpr1;
    Device->SDCR[FMC_SDRAM_BANK2] = tmpr2;
  }  
  
  return (status);
}




/**
  * @brief  Initializes the FMC_SDRAM device timing according to the specified
  *         parameters in the FMC_SDRAM_TimingTypeDef
  * @param  Device: Pointer to SDRAM device instance
  * @param  Timing: Pointer to SDRAM Timing structure
  * @param  Bank: SDRAM bank number   
  * @retval sdram timings status
  */
uint8_t SDRAM_Timing_Init(FMC_SDRAM_TypeDef *Device, FMC_SDRAM_TimingTypeDef *Timing, uint32_t Bank) {
  uint32_t tmpr1 = 0U;
  uint32_t tmpr2 = 0U;
  uint8_t status = 1;
    
  /* Set SDRAM device timing parameters */ 
  if (Bank != FMC_SDRAM_BANK2) { 
    tmpr1 = Device->SDTR[FMC_SDRAM_BANK1];
    tmpr1 = (uint32_t)0;
    tmpr1 |= (uint32_t)(
        ((Timing->LoadToActiveDelay) - 1U)
      | (((Timing->ExitSelfRefreshDelay) - 1U) << 4U)
      | (((Timing->SelfRefreshTime) - 1U) << 8U)
      | (((Timing->RowCycleDelay) - 1U) << 12U)
      | (((Timing->WriteRecoveryTime) - 1U) <<16U)
      | (((Timing->RPDelay) - 1U) << 20U)
      | (((Timing->RCDDelay) - 1U) << 24U)
    );
    Device->SDTR[FMC_SDRAM_BANK1] = tmpr1;
  } else {
    /* FMC_Bank2_SDRAM */
    tmpr1 = Device->SDTR[FMC_SDRAM_BANK1];
    tmpr1 &= ((uint32_t)~(FMC_SDTR1_TRC | FMC_SDTR1_TRP));
    tmpr1 |= (uint32_t)(
        (((Timing->RowCycleDelay) - 1U) << 12U)
      | (((Timing->RPDelay) - 1U) << 20U)
    ); 
    tmpr2 = Device->SDTR[FMC_SDRAM_BANK2];
    tmpr2 = (uint32_t)0;
    tmpr2 |= (uint32_t)(
        (((Timing->LoadToActiveDelay) - 1U)
      | (((Timing->ExitSelfRefreshDelay) - 1U) << 4U)
      | (((Timing->SelfRefreshTime) - 1U) << 8U)
      | (((Timing->WriteRecoveryTime) - 1U) <<16U)
      | (((Timing->RCDDelay) - 1U) << 24U)));   

    Device->SDTR[FMC_SDRAM_BANK1] = tmpr1;
    Device->SDTR[FMC_SDRAM_BANK2] = tmpr2;
  }
  return (status);
}




/**
  * @brief  Program the SDRAM Memory Refresh rate.
  * @param  Device: Pointer to SDRAM device instance  
  * @param  RefreshRate: The SDRAM refresh rate value.
  * @retval None
  */
void SDRAM_ProgramRefreshRate(FMC_SDRAM_TypeDef *Device, uint32_t RefreshRate) {
  /* Set the refresh rate in command register */
  Device->SDRTR |= (RefreshRate<<1U);
}




/**
  * @brief  Set the Number of consecutive SDRAM Memory auto Refresh commands.
  * @param  Device: Pointer to SDRAM device instance  
  * @param  AutoRefreshNumber: Specifies the auto Refresh number.       
  * @retval None
  */
void SDRAM_SetAutoRefreshNumber(FMC_SDRAM_TypeDef *Device, uint32_t AutoRefreshNumber) {
  /* Set the Auto-refresh number in command register */
  Device->SDCMR |= (AutoRefreshNumber << 5U); 
}




/**
  * @brief  Send Command to the FMC SDRAM bank
  * @param  Device: Pointer to SDRAM device instance
  * @param  Command: Pointer to SDRAM command structure   
  * @retval send status
  */  
uint8_t SDRAM_SendCommand(FMC_SDRAM_TypeDef *Device, FMC_SDRAM_CommandTypeDef *Command) {
  __IO uint32_t tmpr = 0U;
  uint8_t status = 1;

  /* Set command register */
  tmpr = (uint32_t)(
      (Command->CommandMode)
    | (Command->CommandTarget)
    | (((Command->AutoRefreshNumber) - 1U) << 5U)
    | ((Command->ModeRegisterDefinition) << 9U)
  );
    
  Device->SDCMR = tmpr;
  while (SDRAM_BUSY_STATUS_CHECK);
  return (status);
}
