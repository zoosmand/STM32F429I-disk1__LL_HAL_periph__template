/* Includes ------------------------------------------------------------------*/
#include "spi.h"









////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Writes 8 bit data via SPI
  * @param  device: pointer to an SPI instance
  * @param  data: a byte to send
  * @retval None
  */
void SPI_Write8b(SPI_TypeDef *device, uint8_t data) {
  LL_SPI_TransmitData8(device, data);
  while (!LL_SPI_IsActiveFlag_TXE(device));
}




/**
  * @brief  Reads 8 bit data via SPI
  * @param  device: pointer to an SPI instance
  * @retval  a received byte
  */
uint8_t SPI_Read8b(SPI_TypeDef *device) {
  while (LL_SPI_IsActiveFlag_RXNE(device));
  uint8_t data = LL_SPI_ReceiveData8(device);
  return (data);
}




/**
  * @brief  Transfers 8 bit data via SPI (read and write in one iteraction)
  * @param  device: pointer to an SPI instance
  * @param  data: a byte to send
  * @retval a received byte
  */
uint8_t SPI_Transfer8b(SPI_TypeDef *device, uint8_t data) {
  LL_SPI_TransmitData8(device, data);
  while (!LL_SPI_IsActiveFlag_TXE(device));

  while (LL_SPI_IsActiveFlag_RXNE(device));
  data = LL_SPI_ReceiveData8(device);
  return (data);
}
