################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/live555/BasicUsageEnvironment/BasicHashTable.cpp \
../src/live555/BasicUsageEnvironment/BasicTaskScheduler.cpp \
../src/live555/BasicUsageEnvironment/BasicTaskScheduler0.cpp \
../src/live555/BasicUsageEnvironment/BasicUsageEnvironment.cpp \
../src/live555/BasicUsageEnvironment/BasicUsageEnvironment0.cpp \
../src/live555/BasicUsageEnvironment/DelayQueue.cpp 

OBJS += \
./src/live555/BasicUsageEnvironment/BasicHashTable.o \
./src/live555/BasicUsageEnvironment/BasicTaskScheduler.o \
./src/live555/BasicUsageEnvironment/BasicTaskScheduler0.o \
./src/live555/BasicUsageEnvironment/BasicUsageEnvironment.o \
./src/live555/BasicUsageEnvironment/BasicUsageEnvironment0.o \
./src/live555/BasicUsageEnvironment/DelayQueue.o 

CPP_DEPS += \
./src/live555/BasicUsageEnvironment/BasicHashTable.d \
./src/live555/BasicUsageEnvironment/BasicTaskScheduler.d \
./src/live555/BasicUsageEnvironment/BasicTaskScheduler0.d \
./src/live555/BasicUsageEnvironment/BasicUsageEnvironment.d \
./src/live555/BasicUsageEnvironment/BasicUsageEnvironment0.d \
./src/live555/BasicUsageEnvironment/DelayQueue.d 


# Each subdirectory must supply rules for building sources it contributes
src/live555/BasicUsageEnvironment/%.o: ../src/live555/BasicUsageEnvironment/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/buffer -I../src/com -I../src/compress -I../src/config -I../src/detect -I../src/GST -I../src/menu -I../src/lk -I../src/pelco -I../src/stich -I../src/mv -I../src/Osd -I/usr/include/freetype2 -I../src/mov/inc -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/include -I../src/file -I../src/gpio -I../src/record -I../src/live555 -I../src/live555/BasicUsageEnvironment -I../src/live555/BasicUsageEnvironment/include -I../src/live555/groupsock -I../src/live555/groupsock/include -I../src/live555/liveMedia -I../src/live555/liveMedia/include -I../src/live555/UsageEnvironment -I../src/live555/UsageEnvironment/include -O3 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_53,code=sm_53 -m64 -odir "src/live555/BasicUsageEnvironment" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/buffer -I../src/com -I../src/compress -I../src/config -I../src/detect -I../src/GST -I../src/menu -I../src/lk -I../src/pelco -I../src/stich -I../src/mv -I../src/Osd -I/usr/include/freetype2 -I../src/mov/inc -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/include -I../src/file -I../src/gpio -I../src/record -I../src/live555 -I../src/live555/BasicUsageEnvironment -I../src/live555/BasicUsageEnvironment/include -I../src/live555/groupsock -I../src/live555/groupsock/include -I../src/live555/liveMedia -I../src/live555/liveMedia/include -I../src/live555/UsageEnvironment -I../src/live555/UsageEnvironment/include -O3 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

