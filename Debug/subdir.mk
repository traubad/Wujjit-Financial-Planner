################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DataAccessLayer.cpp \
../EditIncome.cpp \
../Nav.cpp \
../Wujjit.cpp \
../helloApplication.cpp \
../main.cpp 

OBJS += \
./DataAccessLayer.o \
./EditIncome.o \
./Nav.o \
./Wujjit.o \
./helloApplication.o \
./main.o 

CPP_DEPS += \
./DataAccessLayer.d \
./EditIncome.d \
./Nav.d \
./Wujjit.d \
./helloApplication.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.o: ../main.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include/python2.7 -I/home/adamtraub/workspace/hiredis -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"main.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


