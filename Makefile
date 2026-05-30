#---------------------------------------------------------------------------------
# Clear the default suffixes
#---------------------------------------------------------------------------------
.SUFFIXES:

#---------------------------------------------------------------------------------
# TARGET CONFIGURATION
#---------------------------------------------------------------------------------
TARGET		:= engine3ds
SOURCES		:= source
INCLUDES	:= include
DATA		:= data
GRAPHICS	:= gfx

#---------------------------------------------------------------------------------
# OPTIONS AND FLAGS
#---------------------------------------------------------------------------------
ARCH		:= -march=armv6k -mtune=mpcore -mfloat-abi=hard -mfpu=vfp
DEFINES		:= -DARM11 -D_3DS

CFLAGS		:= -Wall -O2 -mword-relocations \
			   -ffunction-sections -fdata-sections \
			   $(ARCH) $(DEFINES)

CXXFLAGS	:= $(CFLAGS) -std=gnu++17 -fno-rtti -fno-exceptions

ASFLAGS		:= -g $(ARCH)
LDFLAGS		:= -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(TARGET).map

#---------------------------------------------------------------------------------
# LIBRARIES
#---------------------------------------------------------------------------------
LIBS		:= -lc -lm -lctru -lcitro3d

#---------------------------------------------------------------------------------
# PATHS AND TOOLS
#---------------------------------------------------------------------------------
PREFIX		:= arm-none-eabi-
CC			:= $(PREFIX)gcc
CXX			:= $(PREFIX)g++
AS			:= $(PREFIX)as
OBJCOPY		:= $(PREFIX)objcopy
STRIP		:= $(PREFIX)strip
MKDIR		:= mkdir -p
RM			:= rm -rf

#---------------------------------------------------------------------------------
# FILES
#---------------------------------------------------------------------------------
# Automatically find all C, C++, and S (assembly) files in the SOURCES directory
CFILES		:= $(wildcard $(SOURCES)/*.c) $(wildcard $(SOURCES)/**/*.c)
CPPFILES	:= $(wildcard $(SOURCES)/*.cpp) $(wildcard $(SOURCES)/**/*.cpp)
SFILES		:= $(wildcard $(SOURCES)/*.s) $(wildcard $(SOURCES)/**/*.s)

# Object files and dependency files
OBJS		:= $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
OBJS		:= $(addprefix build/, $(notdir $(OBJS)))

DEPS		:= $(OBJS:.o=.d)

#---------------------------------------------------------------------------------
# BUILD RULES
#---------------------------------------------------------------------------------
.PHONY: all clean

all: $(TARGET).3dsx

$(TARGET).3dsx: $(TARGET).elf
	@echo "MK 3DSX"
	@$(MKDIR) $(@D)
	@3dsxgen $< $@

$(TARGET).elf: $(OBJS)
	@echo "LINK $@"
	@$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
	@$(STRIP) $@

# Compile C files
build/%.o: $(SOURCES)/%.c
	@echo "CC $<"
	@$(MKDIR) $(build_dir)
	@$(CC) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

# Compile C++ files
build/%.o: $(SOURCES)/%.cpp
	@echo "CXX $<"
	@$(MKDIR) $(build_dir)
	@$(CXX) $(CXXFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

# Compile Assembly files
build/%.o: $(SOURCES)/%.s
	@echo "AS $<"
	@$(MKDIR) $(build_dir)
	@$(AS) $(ASFLAGS) -c $< -o $@

clean:
	@echo "CLEAN"
	@$(RM) build $(TARGET).elf $(TARGET).3dsx $(TARGET).map

# Include dependency files
-include $(DEPS)
