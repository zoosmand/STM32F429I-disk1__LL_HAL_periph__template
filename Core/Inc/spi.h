#ifndef __SPI_H
#define __SPI_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"





/* Exported functions prototypes ---------------------------------------------*/
void SPI_Write8b(SPI_TypeDef *device, uint8_t data);
uint8_t SPI_Read8b(SPI_TypeDef *device);
uint8_t SPI_Transfer8b(SPI_TypeDef *device, uint8_t data);



#ifdef __cplusplus
}
#endif

#endif /* __SPI_H */
