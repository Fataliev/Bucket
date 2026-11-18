################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/user/Documents/GitHub/Bucket/Core/Src/adc.c \
C:/Users/user/Documents/GitHub/Bucket/Core/Src/dma.c \
C:/Users/user/Documents/GitHub/Bucket/Core/Src/gpio.c \
C:/Users/user/Documents/GitHub/Bucket/Core/Src/i2c.c \
C:/Users/user/Documents/GitHub/Bucket/Core/Src/iwdg.c \
C:/Users/user/Documents/GitHub/Bucket/Core/Src/main.c \
C:/Users/user/Documents/GitHub/Bucket/Core/Src/rtc.c \
C:/Users/user/Documents/GitHub/Bucket/Core/Src/spi.c \
C:/Users/user/Documents/GitHub/Bucket/Core/Src/stm32f1xx_it.c \
../Application/User/Core/syscalls.c \
../Application/User/Core/sysmem.c \
C:/Users/user/Documents/GitHub/Bucket/Core/Src/tim.c \
C:/Users/user/Documents/GitHub/Bucket/Core/Src/usart.c 

OBJS += \
./Application/User/Core/adc.o \
./Application/User/Core/dma.o \
./Application/User/Core/gpio.o \
./Application/User/Core/i2c.o \
./Application/User/Core/iwdg.o \
./Application/User/Core/main.o \
./Application/User/Core/rtc.o \
./Application/User/Core/spi.o \
./Application/User/Core/stm32f1xx_it.o \
./Application/User/Core/syscalls.o \
./Application/User/Core/sysmem.o \
./Application/User/Core/tim.o \
./Application/User/Core/usart.o 

C_DEPS += \
./Application/User/Core/adc.d \
./Application/User/Core/dma.d \
./Application/User/Core/gpio.d \
./Application/User/Core/i2c.d \
./Application/User/Core/iwdg.d \
./Application/User/Core/main.d \
./Application/User/Core/rtc.d \
./Application/User/Core/spi.d \
./Application/User/Core/stm32f1xx_it.d \
./Application/User/Core/syscalls.d \
./Application/User/Core/sysmem.d \
./Application/User/Core/tim.d \
./Application/User/Core/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Core/adc.o: C:/Users/user/Documents/GitHub/Bucket/Core/Src/adc.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/Core/dma.o: C:/Users/user/Documents/GitHub/Bucket/Core/Src/dma.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/Core/gpio.o: C:/Users/user/Documents/GitHub/Bucket/Core/Src/gpio.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/Core/i2c.o: C:/Users/user/Documents/GitHub/Bucket/Core/Src/i2c.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/Core/iwdg.o: C:/Users/user/Documents/GitHub/Bucket/Core/Src/iwdg.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/Core/main.o: C:/Users/user/Documents/GitHub/Bucket/Core/Src/main.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/Core/rtc.o: C:/Users/user/Documents/GitHub/Bucket/Core/Src/rtc.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/Core/spi.o: C:/Users/user/Documents/GitHub/Bucket/Core/Src/spi.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/Core/stm32f1xx_it.o: C:/Users/user/Documents/GitHub/Bucket/Core/Src/stm32f1xx_it.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/Core/%.o Application/User/Core/%.su Application/User/Core/%.cyclo: ../Application/User/Core/%.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/Core/tim.o: C:/Users/user/Documents/GitHub/Bucket/Core/Src/tim.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/Core/usart.o: C:/Users/user/Documents/GitHub/Bucket/Core/Src/usart.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Application-2f-User-2f-Core

clean-Application-2f-User-2f-Core:
	-$(RM) ./Application/User/Core/adc.cyclo ./Application/User/Core/adc.d ./Application/User/Core/adc.o ./Application/User/Core/adc.su ./Application/User/Core/dma.cyclo ./Application/User/Core/dma.d ./Application/User/Core/dma.o ./Application/User/Core/dma.su ./Application/User/Core/gpio.cyclo ./Application/User/Core/gpio.d ./Application/User/Core/gpio.o ./Application/User/Core/gpio.su ./Application/User/Core/i2c.cyclo ./Application/User/Core/i2c.d ./Application/User/Core/i2c.o ./Application/User/Core/i2c.su ./Application/User/Core/iwdg.cyclo ./Application/User/Core/iwdg.d ./Application/User/Core/iwdg.o ./Application/User/Core/iwdg.su ./Application/User/Core/main.cyclo ./Application/User/Core/main.d ./Application/User/Core/main.o ./Application/User/Core/main.su ./Application/User/Core/rtc.cyclo ./Application/User/Core/rtc.d ./Application/User/Core/rtc.o ./Application/User/Core/rtc.su ./Application/User/Core/spi.cyclo ./Application/User/Core/spi.d ./Application/User/Core/spi.o ./Application/User/Core/spi.su ./Application/User/Core/stm32f1xx_it.cyclo ./Application/User/Core/stm32f1xx_it.d ./Application/User/Core/stm32f1xx_it.o ./Application/User/Core/stm32f1xx_it.su ./Application/User/Core/syscalls.cyclo ./Application/User/Core/syscalls.d ./Application/User/Core/syscalls.o ./Application/User/Core/syscalls.su ./Application/User/Core/sysmem.cyclo ./Application/User/Core/sysmem.d ./Application/User/Core/sysmem.o ./Application/User/Core/sysmem.su ./Application/User/Core/tim.cyclo ./Application/User/Core/tim.d ./Application/User/Core/tim.o ./Application/User/Core/tim.su ./Application/User/Core/usart.cyclo ./Application/User/Core/usart.d ./Application/User/Core/usart.o ./Application/User/Core/usart.su

.PHONY: clean-Application-2f-User-2f-Core

