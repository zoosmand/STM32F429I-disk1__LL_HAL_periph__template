/* Includes ------------------------------------------------------------------*/
#include "i2c.h"



/* -------------------------------------------------------------------------- */
/* Private function prototypes -----------------------------------------------*/
/* -------------------------------------------------------------------------- */
static uint8_t I2C_GenerateStart(I2C_TypeDef *device);
static void I2C_GenerateStop(I2C_TypeDef *device);
static uint8_t I2C_SendAddress(I2C_TypeDef *device, uint16_t slaveAddress, I2C_AddressMode addressMode, I2C_Direction dir);









////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Writes 8 bit data via I2C
  * @param  device: pointer to an I2C instance
  * @param  data: a byte to send
  * @retval None
  */
uint8_t I2C_Write(I2C_TypeDef *device, uint8_t slaveAddress, uint8_t *buf, uint16_t len) {
  uint8_t status = 0;

  if (I2C_GenerateStart(device)) {
    // if (I2C_Master_Transmit(device, slaveAddress, &reg, 1)) {
      status = I2C_Master_Transmit(device, slaveAddress, buf, len);
    // }
    I2C_GenerateStop(device);
  }

 return (status);
}




/**
  * @brief  Reads 8 bit data via I2C
  * @param  device: pointer to an I2C instance
  * @retval  a received byte
  */
uint8_t I2C_Read(I2C_TypeDef *device, uint8_t slaveAddress, uint8_t reg, uint8_t *buf, uint16_t len) {
  uint8_t status = 0;

  if (I2C_GenerateStart(device)) {
    if (I2C_Master_Transmit(device, slaveAddress, &reg, 1)) {
      if (I2C_GenerateStart(device)) {
        status = I2C_Master_Receive(device, slaveAddress, buf, len);
      }
    }
    I2C_GenerateStop(device);
  }

  return (status);
}




/**
  * @brief  Transfers 8 bit data via I2C (read and write in one iteraction)
  * @param  device: pointer to an I2C instance
  * @param  data: a byte to send
  * @retval a received byte
  */
uint8_t I2C_Transfer8b(I2C_TypeDef *device, uint8_t addr, uint8_t data) {

  return (data);
}




/**
  * @brief  Transmits in master mode an amount of data.
  * @param  device Pointer to a I2C_TypeDef device.
  * @param  slaveAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  buf Pointer to data buffer
  * @param  len Amount of data to be sent
  * @retval transmit status
  */
uint8_t I2C_Master_Transmit(I2C_TypeDef *device, uint16_t slaveAddress, uint8_t *buf, uint16_t len) {
  uint8_t status = 0;
  uint32_t timeout = 500000;
  
  /* Disable Pos */
  CLR_I2C_BIT(&device->CR1, I2C_CR1_POS_Pos);
  /* Disable Ack */
  CLR_I2C_BIT(&device->CR1, I2C_CR1_ACK_Pos);


  if (I2C_SendAddress(device, slaveAddress, mode_7bit, transmit)) {
    while (len--) {
      device->DR = *buf++;
      while (!GET_I2C_REG(&device->SR1, I2C_SR1_TXE_Pos)) {
        timeout--;
        if (!timeout) {
          status = 0;
          return (status);
        }
      }

      if ((GET_I2C_REG(&device->SR1, I2C_SR1_AF_Pos)) || (GET_I2C_REG(&device->SR1, I2C_SR1_ARLO_Pos)) || (GET_I2C_REG(&device->SR1, I2C_SR1_BERR_Pos))) {
        status = 0;
      } else {
        status = 1;
      }
    }
  }

  return (status);
}






/**
  * @brief  Receives in master mode an amount of data.
  * @param  device Pointer to a I2C_TypeDef device.
  * @param  slaveAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  buf Pointer to data buffer
  * @param  len Amount of data to be sent
  * @retval receive status
  */
uint8_t I2C_Master_Receive(I2C_TypeDef *device, uint16_t slaveAddress, uint8_t *buf, uint16_t len) {
  uint8_t status = 0;
  uint32_t timeout = 500000;
  
  /* Disable Pos */
  CLR_I2C_BIT(&device->CR1, I2C_CR1_POS_Pos);
  /* Enable Ack */
  SET_I2C_BIT(&device->CR1, I2C_CR1_ACK_Pos);
  // CLR_I2C_BIT(&device->CR1, I2C_CR1_ACK_Pos);

  if (I2C_SendAddress(device, slaveAddress, mode_7bit, receive)) {
    while (len--) {
      // last byte without Acking
      if (len == 0) {
        CLR_I2C_BIT(&device->CR1, I2C_CR1_ACK_Pos);
      }

      while (!GET_I2C_REG(&device->SR1, I2C_SR1_RXNE_Pos)) {
        timeout--;
        if (!timeout) {
          status = 0;
          return (status);
        }
      }
      *buf++ = device->DR;

      if ((GET_I2C_REG(&device->SR1, I2C_SR1_AF_Pos)) || (GET_I2C_REG(&device->SR1, I2C_SR1_ARLO_Pos)) || (GET_I2C_REG(&device->SR1, I2C_SR1_BERR_Pos))) {
        status = 0;
      } else {
        status = 1;
      }
    }
  }

  return (status);
}






static uint8_t I2C_SendAddress(I2C_TypeDef *device, uint16_t slaveAddress, I2C_AddressMode addressMode, I2C_Direction dir) {
  uint8_t status = 0;
  uint32_t timeout = 500000;

  if (addressMode == mode_7bit) {
    device->DR = (slaveAddress | dir);
    while (!GET_I2C_REG(&device->SR1, I2C_SR1_ADDR_Pos)) {
      timeout--;
      if (!timeout) {
        status = 0;
        return (status);
      }
    }
    if (GET_I2C_REG(&device->SR2, I2C_SR2_MSL_Pos)) {
      status = 1;
    }
  } else {
    // ToDo. 10-bit address handler
  }

  return (status);
}





static uint8_t I2C_GenerateStart(I2C_TypeDef *device) {
  uint8_t status = 1;
  uint32_t timeout = 500000;

   /* Generate Start or ReStart */
  SET_I2C_BIT(&device->CR1, I2C_CR1_START_Pos);

  while (!GET_I2C_REG(&device->SR1, I2C_SR1_SB_Pos)) {
    timeout--;
    if (!timeout) {
      status = 0;
      return (status);
    }
  }
  
  return (status);
}





static void I2C_GenerateStop(I2C_TypeDef *device) {
  SET_I2C_BIT(&device->CR1, I2C_CR1_STOP_Pos);
}