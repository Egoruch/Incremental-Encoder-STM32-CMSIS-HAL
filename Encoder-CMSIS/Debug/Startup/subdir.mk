################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f030f4px.s 

OBJS += \
./Startup/startup_stm32f030f4px.o 

S_DEPS += \
./Startup/startup_stm32f030f4px.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/startup_stm32f030f4px.o: ../Startup/startup_stm32f030f4px.s
	arm-none-eabi-gcc -mcpu=cortex-m0 -g3 -c -x assembler-with-cpp -MMD -MP -MF"Startup/startup_stm32f030f4px.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

