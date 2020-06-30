#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private typedef -----------------------------------------------------------*/
typedef enum {
  mode_7bit    = 0,
  mode_10bit
} I2C_AddressMode;

typedef enum {
  transmit    = 0,
  receive     = 1
} I2C_Direction;



/* Private macro -------------------------------------------------------------*/
#define SET_I2C_BIT(reg, bit)       SET_PERIPH_BB_VAL((uint32_t)reg, 0, bit, 1)
#define CLR_I2C_BIT(reg, bit)       SET_PERIPH_BB_VAL((uint32_t)reg, 0, bit, 0)
#define GET_I2C_REG(reg, bit)       GET_PERIPH_BB_VAL((uint32_t)reg, 0, bit)




/* Exported functions prototypes ---------------------------------------------*/
uint8_t I2C_Write(I2C_TypeDef *device, uint8_t slaveAddress, uint8_t *buf, uint16_t len);
uint8_t I2C_Read(I2C_TypeDef *device, uint8_t slaveAddress, uint8_t reg, uint8_t *buf, uint16_t len);
uint8_t I2C_Transfer8b(I2C_TypeDef *device, uint8_t addr, uint8_t data);
uint8_t I2C_Master_Transmit(I2C_TypeDef *device, uint16_t slaveAddress, uint8_t *buf, uint16_t len);
uint8_t I2C_Master_Receive(I2C_TypeDef *device, uint16_t slaveAddress, uint8_t *buf, uint16_t len);


#ifdef __cplusplus
}
#endif

#endif /* __I2C_H */
