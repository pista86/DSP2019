################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Hardware/acelerometro.c 

OBJS += \
./Drivers/Hardware/acelerometro.o 

C_DEPS += \
./Drivers/Hardware/acelerometro.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Hardware/acelerometro.o: ../Drivers/Hardware/acelerometro.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xC -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I"C:/Users/pista/STM32CubeIDE/workspace_1.1.0/DSP2019/Drivers/Hardware" -I"C:/Users/pista/STM32CubeIDE/workspace_1.1.0/DSP2019/Drivers/Hardware/Components/lsm303dlhc" -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Hardware/acelerometro.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

