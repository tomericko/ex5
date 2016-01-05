################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ClientConnection.cpp \
../src/TcpConnection.cpp \
../src/UdpConnection.cpp \
../src/clientMain.cpp 

OBJS += \
./src/ClientConnection.o \
./src/TcpConnection.o \
./src/UdpConnection.o \
./src/clientMain.o 

CPP_DEPS += \
./src/ClientConnection.d \
./src/TcpConnection.d \
./src/UdpConnection.d \
./src/clientMain.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
<<<<<<< HEAD
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
=======
	g++ -I"/home/roi/git/ex5/ex5Client/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
>>>>>>> refs/remotes/origin/master
	@echo 'Finished building: $<'
	@echo ' '


