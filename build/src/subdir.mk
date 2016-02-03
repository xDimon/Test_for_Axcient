################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DataStorage.cpp \
../src/MD5.cpp \
../src/TEST_DataStorage.cpp 

OBJS += \
./src/DataStorage.o \
./src/MD5.o \
./src/TEST_DataStorage.o 

CPP_DEPS += \
./src/DataStorage.d \
./src/MD5.d \
./src/TEST_DataStorage.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


