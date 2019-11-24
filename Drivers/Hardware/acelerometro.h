
#ifndef INC_ACELEROMETRO_H_
#define INC_ACELEROMETRO_H_

#include "lsm303dlhc.h"
#include "stm32f4xx_hal.h"

#define I2Cx_TIMEOUT_MAX                        0xA000

typedef enum
{
  ACCELERO_OK = 0,
  ACCELERO_ERROR = 1,
  ACCELERO_TIMEOUT = 2
}ACCELERO_StatusTypeDef;

uint8_t acelerometro_init(I2C_HandleTypeDef* I2c, void* I2c_init);
void acelerometro_getXYZ(int16_t *pDataXYZ);







#endif /* INC_ACELEROMETRO_H_ */

