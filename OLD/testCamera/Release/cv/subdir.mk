################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../cv/test_cam.cpp 

OBJS += \
./cv/test_cam.o 

CPP_DEPS += \
./cv/test_cam.d 


# Each subdirectory must supply rules for building sources it contributes
cv/%.o: ../cv/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -I/usr/include/opencv -O3 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_32,code=sm_32  -odir "cv" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -I/usr/include/opencv -O3 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


