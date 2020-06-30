/* Includes ------------------------------------------------------------------*/
#include "ts.h"


/* Private variables ---------------------------------------------------------*/
static TouchScreen_TypeDef ts = {
  .device         = I2C3,
  .address        = TS_STMPE811_I2C_ADDR,
  .lock           = UNLOCK
};
static uint8_t txBuf[4];
static uint8_t rxBuf[4];







////////////////////////////////////////////////////////////////////////////////

void TouchScreen_Handler(void) {
  uint8_t reg = 0x04;

  txBuf[0] = reg;
  txBuf[1] = 0x0e;
  
  
  TouchScreen_Write(&ts, txBuf, 2);
  TouchScreen_Read(&ts, 0x02, rxBuf, 3);
}




uint8_t TouchScreen_Read(TouchScreen_TypeDef *device, uint8_t reg, uint8_t *buf, uint8_t len) {
  uint8_t status = 0;
  device->lock = LOCK;

  status = I2C_Read(device->device, device->address, reg, buf, len);
  
  device->lock = UNLOCK;

  return (status);
}



uint8_t TouchScreen_Write(TouchScreen_TypeDef *device, uint8_t *buf, uint8_t len) {
  uint8_t status = 0;
  device->lock = LOCK;

  status = I2C_Write(device->device, device->address, buf, len);
  
  device->lock = UNLOCK;

  return (status);
}