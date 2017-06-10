################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AVX\ Test\ Single\ Precision.cpp 

OBJS += \
./src/AVX\ Test\ Single\ Precision.o 

CPP_DEPS += \
./src/AVX\ Test\ Single\ Precision.d 


# Each subdirectory must supply rules for building sources it contributes
src/AVX\ Test\ Single\ Precision.o: ../src/AVX\ Test\ Single\ Precision.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -O3 -mxsave -mavx  -ftree-vectorize -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/AVX Test Single Precision.d" -MT"src/AVX\ Test\ Single\ Precision.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


