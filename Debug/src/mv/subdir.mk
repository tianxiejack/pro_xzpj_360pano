################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/mv/FrameDifference.cpp \
../src/mv/LBFuzzyGaussian.cpp \
../src/mv/StaticFrameDifference.cpp \
../src/mv/TwoPoints.cpp \
../src/mv/two_points.cpp 

OBJS += \
./src/mv/FrameDifference.o \
./src/mv/LBFuzzyGaussian.o \
./src/mv/StaticFrameDifference.o \
./src/mv/TwoPoints.o \
./src/mv/two_points.o 

CPP_DEPS += \
./src/mv/FrameDifference.d \
./src/mv/LBFuzzyGaussian.d \
./src/mv/StaticFrameDifference.d \
./src/mv/TwoPoints.d \
./src/mv/two_points.d 


# Each subdirectory must supply rules for building sources it contributes
src/mv/%.o: ../src/mv/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/buffer -I../src/com -I../src/compress -I../src/config -I../src/detect -I../src/GST -I../src/menu -I../src/lk -I../src/pelco -I../src/stich -I../src/mv -I../src/Osd -I../src/mov/inc -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/include -G -g -O0 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_53,code=sm_53 -m64 -odir "src/mv" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/buffer -I../src/com -I../src/compress -I../src/config -I../src/detect -I../src/GST -I../src/menu -I../src/lk -I../src/pelco -I../src/stich -I../src/mv -I../src/Osd -I../src/mov/inc -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/include -G -g -O0 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


