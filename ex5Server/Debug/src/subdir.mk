################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Actor.cpp \
../src/Director.cpp \
../src/Movie.cpp \
../src/MoviesSystem.cpp \
../src/Producer.cpp \
../src/Professional.cpp \
../src/ScreenWriter.cpp \
../src/Server.cpp \
../src/SortStaffByAge.cpp \
../src/SortStaffById.cpp \
../src/SortStaffByMoviesQuantity.cpp \
../src/SortingStaffType.cpp \
../src/StaffComparator.cpp \
../src/TCPServer.cpp \
../src/Type.cpp \
../src/UDPServer.cpp \
../src/serverMain.cpp 

OBJS += \
./src/Actor.o \
./src/Director.o \
./src/Movie.o \
./src/MoviesSystem.o \
./src/Producer.o \
./src/Professional.o \
./src/ScreenWriter.o \
./src/Server.o \
./src/SortStaffByAge.o \
./src/SortStaffById.o \
./src/SortStaffByMoviesQuantity.o \
./src/SortingStaffType.o \
./src/StaffComparator.o \
./src/TCPServer.o \
./src/Type.o \
./src/UDPServer.o \
./src/serverMain.o 

CPP_DEPS += \
./src/Actor.d \
./src/Director.d \
./src/Movie.d \
./src/MoviesSystem.d \
./src/Producer.d \
./src/Professional.d \
./src/ScreenWriter.d \
./src/Server.d \
./src/SortStaffByAge.d \
./src/SortStaffById.d \
./src/SortStaffByMoviesQuantity.d \
./src/SortingStaffType.d \
./src/StaffComparator.d \
./src/TCPServer.d \
./src/Type.d \
./src/UDPServer.d \
./src/serverMain.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/roi/git/ex5/ex5Server/include" -I"/home/roi/git/ex5/ex5Server/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


