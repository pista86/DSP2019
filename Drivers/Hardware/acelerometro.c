#include "acelerometro.h"

typedef void (*I2c_init_t)(void);


static ACCELERO_IO_t acelero_io;
static uint32_t I2cxTimeout = I2Cx_TIMEOUT_MAX;

static I2C_HandleTypeDef *I2cHandle;
static ACCELERO_DrvTypeDef *AccelerometerDrv;
static I2c_init_t I2Cx_Init;

static void I2Cx_Error(void);

static void COMPASSACCELERO_IO_Init(void);
static void COMPASSACCELERO_IO_ITConfig(void);
static void COMPASSACCELERO_IO_Write(uint16_t DeviceAddr, uint8_t RegisterAddr,
		uint8_t Value);
static uint8_t COMPASSACCELERO_IO_Read(uint16_t DeviceAddr,
		uint8_t RegisterAddr);

uint8_t acelerometro_init(I2C_HandleTypeDef *I2c, void *I2c_init) {

	I2cHandle = I2c;
	I2Cx_Init = (I2c_init_t) I2c_init;

	acelero_io.COMPASSACCELERO_IO_Init = COMPASSACCELERO_IO_Init;
	acelero_io.COMPASSACCELERO_IO_ITConfig = COMPASSACCELERO_IO_ITConfig;
	acelero_io.COMPASSACCELERO_IO_Write = COMPASSACCELERO_IO_Write;
	acelero_io.COMPASSACCELERO_IO_Read = COMPASSACCELERO_IO_Read;

	uint8_t ret = ACCELERO_ERROR;
	uint16_t ctrl = 0x0000;
	ACCELERO_InitTypeDef LSM303DLHC_InitStructure;
	ACCELERO_FilterConfigTypeDef LSM303DLHC_FilterStructure = { 0, 0, 0, 0 };

	if (Lsm303dlhcDrv.ReadID() == I_AM_LMS303DLHC) {
		/* Initialize the Accelerometer driver structure */
		AccelerometerDrv = &Lsm303dlhcDrv;

		/* MEMS configuration ----------------------------------------------------*/
		/* Fill the Accelerometer structure */
		LSM303DLHC_InitStructure.Power_Mode = LSM303DLHC_NORMAL_MODE;
		LSM303DLHC_InitStructure.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
		LSM303DLHC_InitStructure.Axes_Enable = LSM303DLHC_AXES_ENABLE;
		LSM303DLHC_InitStructure.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
		LSM303DLHC_InitStructure.BlockData_Update =
				LSM303DLHC_BlockUpdate_Continous;
		LSM303DLHC_InitStructure.Endianness = LSM303DLHC_BLE_LSB;
		LSM303DLHC_InitStructure.High_Resolution = LSM303DLHC_HR_ENABLE;

		/* Configure MEMS: data rate, power mode, full scale and axes */
		ctrl |= (LSM303DLHC_InitStructure.Power_Mode
				| LSM303DLHC_InitStructure.AccOutput_DataRate
				| LSM303DLHC_InitStructure.Axes_Enable);

		ctrl |= ((LSM303DLHC_InitStructure.BlockData_Update
				| LSM303DLHC_InitStructure.Endianness
				| LSM303DLHC_InitStructure.AccFull_Scale
				| LSM303DLHC_InitStructure.High_Resolution) << 8);

		/* Configure the Accelerometer main parameters */
		AccelerometerDrv->Init(ctrl, acelero_io);

		/* Fill the Accelerometer LPF structure */
		LSM303DLHC_FilterStructure.HighPassFilter_Mode_Selection =
				LSM303DLHC_HPM_NORMAL_MODE;
		LSM303DLHC_FilterStructure.HighPassFilter_CutOff_Frequency =
				LSM303DLHC_HPFCF_16;
		LSM303DLHC_FilterStructure.HighPassFilter_AOI1 =
				LSM303DLHC_HPF_AOI1_DISABLE;
		LSM303DLHC_FilterStructure.HighPassFilter_AOI2 =
				LSM303DLHC_HPF_AOI2_DISABLE;

		/* Configure MEMS: mode, cutoff frquency, Filter status, Click, AOI1 and AOI2 */
		ctrl =
				(uint8_t) (LSM303DLHC_FilterStructure.HighPassFilter_Mode_Selection
						|\
 LSM303DLHC_FilterStructure.HighPassFilter_CutOff_Frequency
						|\
 LSM303DLHC_FilterStructure.HighPassFilter_AOI1
						|\
 LSM303DLHC_FilterStructure.HighPassFilter_AOI2);

		/* Configure the Accelerometer LPF main parameters */
		AccelerometerDrv->FilterConfig(ctrl);

		ret = ACCELERO_OK;
	} else {
		ret = ACCELERO_ERROR;
	}

	return ret;

}

void acelerometro_getXYZ(int16_t *pDataXYZ)
{
  int16_t SwitchXY = 0;

  if(AccelerometerDrv->GetXYZ!= NULL)
  {
    AccelerometerDrv->GetXYZ(pDataXYZ);

    /* Switch X and Y Axes in case of LSM303DLHC MEMS */
    if(AccelerometerDrv == &Lsm303dlhcDrv)
    {
      SwitchXY  = pDataXYZ[0];
      pDataXYZ[0] = pDataXYZ[1];

      /* Invert Y Axis to be conpliant with LIS3DSH */
      pDataXYZ[1] = -SwitchXY;
    }
  }
}

static void COMPASSACCELERO_IO_Write(uint16_t DeviceAddr, uint8_t RegisterAddr,
		uint8_t Value) {
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(I2cHandle, DeviceAddr, (uint16_t) RegisterAddr,
	I2C_MEMADD_SIZE_8BIT, &Value, 1, I2cxTimeout);

	/* Check the communication status */
	if (status != HAL_OK) {
		/* Execute user timeout callback */
		I2Cx_Error();
	}
}

static uint8_t COMPASSACCELERO_IO_Read(uint16_t DeviceAddr, uint8_t RegisterAddr) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t value = 0;

	status = HAL_I2C_Mem_Read(I2cHandle, DeviceAddr, RegisterAddr,
	I2C_MEMADD_SIZE_8BIT, &value, 1, I2cxTimeout);

	/* Check the communication status */
	if (status != HAL_OK) {
		/* Execute user timeout callback */
		I2Cx_Error();
	}
	return value;
}

static void COMPASSACCELERO_IO_Init(void) {

// No implementado

}

static void COMPASSACCELERO_IO_ITConfig(void) {

// No implementado
}

/**
 * @brief  I2Cx error treatment function.
 */
static void I2Cx_Error(void) {
	/* De-initialize the I2C comunication BUS */
	HAL_I2C_DeInit(I2cHandle);

	/* Re- Initiaize the I2C comunication BUS */
	I2Cx_Init();
}

