################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../src/cuda.cu 

CPP_SRCS += \
../src/Comcontrl.cpp \
../src/Cylindrical.cpp \
../src/Displaybase.cpp \
../src/Displayer.cpp \
../src/FileRW.cpp \
../src/Gldisplay.cpp \
../src/Gyroprocess.cpp \
../src/Homography.cpp \
../src/ImageProcess.cpp \
../src/MPU9250.cpp \
../src/Render.cpp \
../src/Stich.cpp \
../src/cuda_mem.cpp \
../src/extra.cpp \
../src/gst_ring_buffer.cpp \
../src/gyro.cpp \
../src/gyro_calibrate.cpp \
../src/gyro_math.cpp \
../src/gyro_protocol.cpp \
../src/gyro_quater.cpp \
../src/main.cpp \
../src/main_cap.cpp \
../src/main_pano.cpp \
../src/osa_image_queue.cpp \
../src/plantformcontrl.cpp 

OBJS += \
./src/Comcontrl.o \
./src/Cylindrical.o \
./src/Displaybase.o \
./src/Displayer.o \
./src/FileRW.o \
./src/Gldisplay.o \
./src/Gyroprocess.o \
./src/Homography.o \
./src/ImageProcess.o \
./src/MPU9250.o \
./src/Render.o \
./src/Stich.o \
./src/cuda.o \
./src/cuda_mem.o \
./src/extra.o \
./src/gst_ring_buffer.o \
./src/gyro.o \
./src/gyro_calibrate.o \
./src/gyro_math.o \
./src/gyro_protocol.o \
./src/gyro_quater.o \
./src/main.o \
./src/main_cap.o \
./src/main_pano.o \
./src/osa_image_queue.o \
./src/plantformcontrl.o 

CU_DEPS += \
./src/cuda.d 

CPP_DEPS += \
./src/Comcontrl.d \
./src/Cylindrical.d \
./src/Displaybase.d \
./src/Displayer.d \
./src/FileRW.d \
./src/Gldisplay.d \
./src/Gyroprocess.d \
./src/Homography.d \
./src/ImageProcess.d \
./src/MPU9250.d \
./src/Render.d \
./src/Stich.d \
./src/cuda_mem.d \
./src/extra.d \
./src/gst_ring_buffer.d \
./src/gyro.d \
./src/gyro_calibrate.d \
./src/gyro_math.d \
./src/gyro_protocol.d \
./src/gyro_quater.d \
./src/main.d \
./src/main_cap.d \
./src/main_pano.d \
./src/osa_image_queue.d \
./src/plantformcontrl.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/buffer -I../src/com -I../src/compress -I../src/config -I../src/detect -I../src/GST -I../src/menu -I../src/lk -I../src/pelco -I../src/stich -I../src/mv -I../src/Osd -I/usr/include/freetype2 -I../src/mov/inc -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/include -I../src/file -I../src/gpio -I../src/record -I../src/live555 -I../src/live555/BasicUsageEnvironment -I../src/live555/BasicUsageEnvironment/include -I../src/live555/groupsock -I../src/live555/groupsock/include -I../src/live555/liveMedia -I../src/live555/liveMedia/include -I../src/live555/UsageEnvironment -I../src/live555/UsageEnvironment/include -I../src/rtsp -I../src/msg/include -I../src/com/include -I../src/status -I../src/manager -I../src/DxTimer/include -I../src/store -I../src/configfile -O3 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_53,code=sm_53 -m64 -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/buffer -I../src/com -I../src/compress -I../src/config -I../src/detect -I../src/GST -I../src/menu -I../src/lk -I../src/pelco -I../src/stich -I../src/mv -I../src/Osd -I/usr/include/freetype2 -I../src/mov/inc -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/include -I../src/file -I../src/gpio -I../src/record -I../src/live555 -I../src/live555/BasicUsageEnvironment -I../src/live555/BasicUsageEnvironment/include -I../src/live555/groupsock -I../src/live555/groupsock/include -I../src/live555/liveMedia -I../src/live555/liveMedia/include -I../src/live555/UsageEnvironment -I../src/live555/UsageEnvironment/include -I../src/rtsp -I../src/msg/include -I../src/com/include -I../src/status -I../src/manager -I../src/DxTimer/include -I../src/store -I../src/configfile -O3 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/buffer -I../src/com -I../src/compress -I../src/config -I../src/detect -I../src/GST -I../src/menu -I../src/lk -I../src/pelco -I../src/stich -I../src/mv -I../src/Osd -I/usr/include/freetype2 -I../src/mov/inc -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/include -I../src/file -I../src/gpio -I../src/record -I../src/live555 -I../src/live555/BasicUsageEnvironment -I../src/live555/BasicUsageEnvironment/include -I../src/live555/groupsock -I../src/live555/groupsock/include -I../src/live555/liveMedia -I../src/live555/liveMedia/include -I../src/live555/UsageEnvironment -I../src/live555/UsageEnvironment/include -I../src/rtsp -I../src/msg/include -I../src/com/include -I../src/status -I../src/manager -I../src/DxTimer/include -I../src/store -I../src/configfile -O3 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_53,code=sm_53 -m64 -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/buffer -I../src/com -I../src/compress -I../src/config -I../src/detect -I../src/GST -I../src/menu -I../src/lk -I../src/pelco -I../src/stich -I../src/mv -I../src/Osd -I/usr/include/freetype2 -I../src/mov/inc -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/include -I../src/file -I../src/gpio -I../src/record -I../src/live555 -I../src/live555/BasicUsageEnvironment -I../src/live555/BasicUsageEnvironment/include -I../src/live555/groupsock -I../src/live555/groupsock/include -I../src/live555/liveMedia -I../src/live555/liveMedia/include -I../src/live555/UsageEnvironment -I../src/live555/UsageEnvironment/include -I../src/rtsp -I../src/msg/include -I../src/com/include -I../src/status -I../src/manager -I../src/DxTimer/include -I../src/store -I../src/configfile -O3 --compile --relocatable-device-code=false -gencode arch=compute_53,code=compute_53 -gencode arch=compute_53,code=sm_53 -m64 -ccbin aarch64-linux-gnu-g++  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


