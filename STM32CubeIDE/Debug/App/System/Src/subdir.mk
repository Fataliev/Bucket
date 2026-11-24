################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/System/Src/super_cycle.c 

OBJS += \
./App/System/Src/super_cycle.o 

C_DEPS += \
./App/System/Src/super_cycle.d 


# Each subdirectory must supply rules for building sources it contributes
App/System/Src/%.o App/System/Src/%.su App/System/Src/%.cyclo: ../App/System/Src/%.c App/System/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-App-2f-System-2f-Src

clean-App-2f-System-2f-Src:
	-$(RM) ./App/System/Src/super_cycle.cyclo ./App/System/Src/super_cycle.d ./App/System/Src/super_cycle.o ./App/System/Src/super_cycle.su

.PHONY: clean-App-2f-System-2f-Src

