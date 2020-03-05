#ifndef __SDRAM_H
#define __SDRAM_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private defines -----------------------------------------------------------*/
#define FMC_SDSR_Offset                           (uint16_t)0x0158

#define SDRAM_START_ADDR                          (uint32_t)0xd0000000
#define SDRAM_MAX_ADDR                            0x800000

#define SDRAM_MODEREG_BURST_LENGTH_1              (uint16_t)0x0000
#define SDRAM_MODEREG_BURST_LENGTH_2              (uint16_t)0x0001
#define SDRAM_MODEREG_BURST_LENGTH_4              (uint16_t)0x0002
#define SDRAM_MODEREG_BURST_LENGTH_8              (uint16_t)0x0004
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL       (uint16_t)0x0000
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED      (uint16_t)0x0008
#define SDRAM_MODEREG_CAS_LATENCY_2               (uint16_t)0x0020
#define SDRAM_MODEREG_CAS_LATENCY_3               (uint16_t)0x0030
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD     (uint16_t)0x0000
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED  (uint16_t)0x0000
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE      (uint16_t)0x0200


/* Exported macro ------------------------------------------------------------*/
#define SDRAM_BUSY_STATUS_CHECK                   (GET_PERIPH_BB_VAL(FMC_R_BASE, FMC_SDSR_Offset, FMC_SDSR_BUSY_Pos))


/* Exported functions prototypes ---------------------------------------------*/
void SDRAM_Write8b(uint32_t addr, uint8_t data);
uint8_t SDRAM_Read8b(uint32_t addr);
void SDRAM_Write16b(uint32_t addr, uint16_t data);
uint16_t SDRAM_Read16b(uint32_t addr);
void SDRAM_Write32b(uint32_t addr, uint32_t data);
uint32_t SDRAM_Read32b(uint32_t addr);
uint8_t SDRAM_ReadBuffer8b(uint32_t addr, uint32_t cnt, uint8_t *buf);
uint8_t SDRAM_WriteBuffer8b(uint32_t addr, uint32_t cnt, uint8_t *buf);
uint8_t SDRAM_ReadBuffer(uint32_t addr, uint32_t cnt, uint32_t *buf);
uint8_t SDRAM_WriteBuffer(uint32_t addr, uint32_t cnt, uint32_t *buf);

uint8_t SDRAM_Init(FMC_SDRAM_TypeDef *Device, FMC_SDRAM_InitTypeDef *Init);
uint8_t SDRAM_Timing_Init(FMC_SDRAM_TypeDef *Device, FMC_SDRAM_TimingTypeDef *Timing, uint32_t Bank);
void SDRAM_ProgramRefreshRate(FMC_SDRAM_TypeDef *Device, uint32_t RefreshRate);
void SDRAM_SetAutoRefreshNumber(FMC_SDRAM_TypeDef *Device, uint32_t AutoRefreshNumber);
uint8_t SDRAM_SendCommand(FMC_SDRAM_TypeDef *Device, FMC_SDRAM_CommandTypeDef *Command);


#ifdef __cplusplus
}
#endif

#endif /* __SDRAM_H */