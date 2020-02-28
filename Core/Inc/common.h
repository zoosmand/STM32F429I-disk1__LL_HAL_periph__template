#ifndef __COMMON_H
#define __COMMON_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
extern uint32_t delay_tmp;
    
/* Private typedef -----------------------------------------------------------*/
struct __FILE {
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};

/* Private defines -----------------------------------------------------------*/
#define GPIO_IDR      (uint16_t)0x0010
#define GPIO_ODR      (uint16_t)0x0014
#define GPIO_BSRR     (uint16_t)0x0018



/* Private macro -------------------------------------------------------------*/
#define GET_PERIPH_BB_ADDR(addr, periphOffset, wordOffset)      (PERIPH_BB_BASE + ((addr + periphOffset) * 32U) + (wordOffset * 4U))
#define SET_PERIPH_BB_VAL(addr, periphOffset, wordOffset, key)  Set_BitBandVal((GET_PERIPH_BB_ADDR(addr, periphOffset, wordOffset)), key)
#define GET_PERIPH_BB_VAL(addr, periphOffset, wordOffset)       (Get_BitBandVal((GET_PERIPH_BB_ADDR(addr, periphOffset, wordOffset))))

#define GET_SRAM_BB_ADDR(addr, periphOffset, wordOffset)        (SRAM_BB_BASE + ((addr + periphOffset) * 32U) + (wordOffset * 4U))
#define SET_SRAM_BB_VAL(addr, periphOffset, wordOffset, key)    Set_BitBandVal((GET_SRAM_BB_ADDR(addr, periphOffset, wordOffset)), key)
#define GET_SRAM_BB_VAL(addr, periphOffset, wordOffset)         (Get_BitBandVal((GET_SRAM_BB_ADDR(addr, periphOffset, wordOffset))))

/* Exported macro ------------------------------------------------------------*/
#define FLAG_SET(registry, flag)        SET_SRAM_BB_VAL((uint32_t)&registry, 0, flag, 1)
#define FLAG_CLR(registry, flag)        SET_SRAM_BB_VAL((uint32_t)&registry, 0, flag, 0)
#define FLAG_CHECK(registry, flag)      (GET_SRAM_BB_VAL((uint32_t)&registry, 0, flag))

#define PIN_H(port, pinSource)          SET_PERIPH_BB_VAL((uint32_t)port, GPIO_ODR, pinSource, 1)
#define PIN_L(port, pinSource)          SET_PERIPH_BB_VAL((uint32_t)port, GPIO_ODR, pinSource, 0)
#define PIN_LEVEL(port, pinSource)      (GET_PERIPH_BB_VAL((uint32_t)port, GPIO_IDR, pinSource))





/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void LED_Blink(GPIO_TypeDef* port, uint16_t pinSource);



#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H */