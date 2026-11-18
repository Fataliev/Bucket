################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/ad7799.c \
../Drivers/Src/adg888.c 

OBJS += \
./Drivers/Src/ad7799.o \
./Drivers/Src/adg888.o 

C_DEPS += \
./Drivers/Src/ad7799.d \
./Drivers/Src/adg888.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/%.o Drivers/Src/%.su Drivers/Src/%.cyclo: ../Drivers/Src/%.c Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Src

clean-Drivers-2f-Src:
	-$(RM) ./Drivers/Src/ad7799.cyclo ./Drivers/Src/ad7799.d ./Drivers/Src/ad7799.o ./Drivers/Src/ad7799.su ./Drivers/Src/adg888.cyclo ./Drivers/Src/adg888.d ./Drivers/Src/adg888.o ./Drivers/Src/adg888.su

.PHONY: clean-Drivers-2f-Src

