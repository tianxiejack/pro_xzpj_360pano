################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/store/subdir.mk
-include src/stich/subdir.mk
-include src/status/subdir.mk
-include src/rtsp/subdir.mk
-include src/record/subdir.mk
-include src/subdir.mk
-include src/pelco/subdir.mk
-include src/mv/subdir.mk
-include src/mv/lb/subdir.mk
-include src/msg/src/subdir.mk
-include src/mov/src/subdir.mk
-include src/menu/subdir.mk
-include src/manager/subdir.mk
-include src/lk/subdir.mk
-include src/live555/liveMedia/subdir.mk
-include src/live555/groupsock/subdir.mk
-include src/live555/UsageEnvironment/subdir.mk
-include src/live555/BasicUsageEnvironment/subdir.mk
-include src/gpio/subdir.mk
-include src/file/subdir.mk
-include src/detect/subdir.mk
-include src/configfile/subdir.mk
-include src/config/subdir.mk
-include src/compress/subdir.mk
-include src/com/subdir.mk
-include src/capture/subdir.mk
-include src/buffer/subdir.mk
-include src/Osd/subdir.mk
-include src/OSA_CAP/src/subdir.mk
-include src/GST/subdir.mk
-include src/GLTools/src/subdir.mk
-include src/DxTimer/src/subdir.mk

OS_SUFFIX := $(subst Linux,linux,$(subst Darwin/x86_64,darwin,$(shell uname -s)/$(shell uname -m)))

-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CU_DEPS)),)
-include $(CU_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# OpenGL+GLUT OS-specific define
ifeq ($(shell uname -s),Darwin)
GLUT_LIBS := -Xlinker -framework -Xlinker GLUT -Xlinker -framework -Xlinker OpenGL
else
GLUT_LIBS := -lGL -lGLU -lglut 
endif


# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: 360pano

# Tool invocations
360pano: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: NVCC Linker'
	/usr/local/cuda-8.0/bin/nvcc --cudart static -L../lib -Xlinker -rpath=/usr/lib/aarch64-linux-gnu/tegra --relocatable-device-code=false -gencode arch=compute_53,code=compute_53 -gencode arch=compute_53,code=sm_53 -m64 -ccbin aarch64-linux-gnu-g++ -link -o  "360pano" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CU_DEPS)$(CPP_DEPS)$(C_DEPS) 360pano
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
