################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../testCamera.cu 

OBJS += \
./testCamera.o 

CU_DEPS += \
./testCamera.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -I/usr/include/opencv -O3 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_32,code=sm_32  -odir "." -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -I/usr/include/opencv -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_32,code=sm_32  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


