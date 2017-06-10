################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AVX\ Test.cpp 

OBJS += \
./src/AVX\ Test.o 

CPP_DEPS += \
./src/AVX\ Test.d 


# Each subdirectory must supply rules for building sources it contributes
src/AVX\ Test.o: ../src/AVX\ Test.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/AVX Test.d" -MT"src/AVX\ Test.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


