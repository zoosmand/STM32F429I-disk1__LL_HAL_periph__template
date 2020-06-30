#ifndef __TS_H
#define __TS_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"



/* Private typedef -----------------------------------------------------------*/
typedef enum {
  UNLOCK    = 0,
  LOCK
} LockState;

typedef struct {
  I2C_TypeDef*  device;
  uint8_t       address;
  LockState     lock;
} TouchScreen_TypeDef;






/* Private defines -----------------------------------------------------------*/
#define TS_STMPE811_I2C_ADDR             (uint8_t)0x82




/* Exported functions prototypes ---------------------------------------------*/
void TouchScreen_Handler(void);
uint8_t TouchScreen_Read(TouchScreen_TypeDef *device, uint8_t reg, uint8_t *buf, uint8_t len);
uint8_t TouchScreen_Write(TouchScreen_TypeDef *device, uint8_t *buf, uint8_t len);


#ifdef __cplusplus
}
#endif

#endif /* __TS_H */
