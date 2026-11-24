################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Platform/Src/bsp_gpio.c \
../Platform/Src/onewire_uart.c \
../Platform/Src/periph_spi.c \
../Platform/Src/periph_usart.c \
../Platform/Src/platform_events.c \
../Platform/Src/platform_isr.c \
../Platform/Src/platform_tick.c \
../Platform/Src/usb_ft232.c 

OBJS += \
./Platform/Src/bsp_gpio.o \
./Platform/Src/onewire_uart.o \
./Platform/Src/periph_spi.o \
./Platform/Src/periph_usart.o \
./Platform/Src/platform_events.o \
./Platform/Src/platform_isr.o \
./Platform/Src/platform_tick.o \
./Platform/Src/usb_ft232.o 

C_DEPS += \
./Platform/Src/bsp_gpio.d \
./Platform/Src/onewire_uart.d \
./Platform/Src/periph_spi.d \
./Platform/Src/periph_usart.d \
./Platform/Src/platform_events.d \
./Platform/Src/platform_isr.d \
./Platform/Src/platform_tick.d \
./Platform/Src/usb_ft232.d 


# Each subdirectory must supply rules for building sources it contributes
Platform/Src/%.o Platform/Src/%.su Platform/Src/%.cyclo: ../Platform/Src/%.c Platform/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../../Core/Inc -I"../../STM32CubeIDE/Platform/Inc" -I"../../STM32CubeIDE/App/Measurement/Inc" -I"../../STM32CubeIDE/App/Protocols/Inc" -I"../../STM32CubeIDE/App/System/Inc" -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/Drivers/Inc" -I"C:/Users/user/Documents/GitHub/Bucket/STM32CubeIDE/App/Measurement/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Platform-2f-Src

clean-Platform-2f-Src:
	-$(RM) ./Platform/Src/bsp_gpio.cyclo ./Platform/Src/bsp_gpio.d ./Platform/Src/bsp_gpio.o ./Platform/Src/bsp_gpio.su ./Platform/Src/onewire_uart.cyclo ./Platform/Src/onewire_uart.d ./Platform/Src/onewire_uart.o ./Platform/Src/onewire_uart.su ./Platform/Src/periph_spi.cyclo ./Platform/Src/periph_spi.d ./Platform/Src/periph_spi.o ./Platform/Src/periph_spi.su ./Platform/Src/periph_usart.cyclo ./Platform/Src/periph_usart.d ./Platform/Src/periph_usart.o ./Platform/Src/periph_usart.su ./Platform/Src/platform_events.cyclo ./Platform/Src/platform_events.d ./Platform/Src/platform_events.o ./Platform/Src/platform_events.su ./Platform/Src/platform_isr.cyclo ./Platform/Src/platform_isr.d ./Platform/Src/platform_isr.o ./Platform/Src/platform_isr.su ./Platform/Src/platform_tick.cyclo ./Platform/Src/platform_tick.d ./Platform/Src/platform_tick.o ./Platform/Src/platform_tick.su ./Platform/Src/usb_ft232.cyclo ./Platform/Src/usb_ft232.d ./Platform/Src/usb_ft232.o ./Platform/Src/usb_ft232.su

.PHONY: clean-Platform-2f-Src

