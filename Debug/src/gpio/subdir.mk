################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gpio/gpio_rdwr.cpp 

OBJS += \
./src/gpio/gpio_rdwr.o 

CPP_DEPS += \
./src/gpio/gpio_rdwr.d 


# Each subdirectory must supply rules for building sources it contributes
src/gpio/%.o: ../src/gpio/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
<<<<<<< HEAD
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/mov/inc -I../src/Osd/ -I/usr/include/freetype2/ -I../src/pelco -I../src/com -I../src/lk -I../src/mv -I../src/config -I../src/menu -I../src/stich -I../src/buffer -I../src/detect -I../src/GST -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/include -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I../src/compress -G -g -O0 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_53,code=sm_53 -m64 -odir "src/gpio" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/mov/inc -I../src/Osd/ -I/usr/include/freetype2/ -I../src/pelco -I../src/com -I../src/lk -I../src/mv -I../src/config -I../src/menu -I../src/stich -I../src/buffer -I../src/detect -I../src/GST -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/include -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I../src/compress -G -g -O0 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
=======
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/mov/inc -I../src/Osd/ -I/usr/include/freetype2/ -I../src/pelco -I../src/com -I../src/lk -I../src/mv -I../src/config -I../src/menu -I../src/stich -I../src/buffer -I../src/detect -G -g -O0 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_53,code=sm_53 -m64 -odir "src/gpio" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -I../include -I../src/OSA_CAP/inc -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../src/encTrans -I../src/capture -I../src/GLTools/include -I../src -I../src/mov/inc -I../src/Osd/ -I/usr/include/freetype2/ -I../src/pelco -I../src/com -I../src/lk -I../src/mv -I../src/config -I../src/menu -I../src/stich -I../src/buffer -I../src/detect -G -g -O0 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
>>>>>>> refs/remotes/eclipse_auto/master
	@echo 'Finished building: $<'
	@echo ' '


