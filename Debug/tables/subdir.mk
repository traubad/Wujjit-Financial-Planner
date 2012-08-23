################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tables/EditableCell.cpp \
../tables/EditableTable.cpp \
../tables/IncomeTable.cpp 

OBJS += \
./tables/EditableCell.o \
./tables/EditableTable.o \
./tables/IncomeTable.o 

CPP_DEPS += \
./tables/EditableCell.d \
./tables/EditableTable.d \
./tables/IncomeTable.d 


# Each subdirectory must supply rules for building sources it contributes
tables/%.o: ../tables/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


