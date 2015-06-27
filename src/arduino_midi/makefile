#
#             LUFA Library
#     Copyright (C) Dean Camera, 2014.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega16u2
ARCH         = AVR8
BOARD        = UNO
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = arduino_midi
SRC          = $(TARGET).c Descriptors.c $(LUFA_SRC_USB)
LUFA_PATH    = ../../LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     =

# Specifically for the Arduino Due
CC_FLAGS += -DAVR_RESET_LINE_PORT="PORTC"
CC_FLAGS += -DAVR_RESET_LINE_DDR="DDRC"
CC_FLAGS += -DAVR_RESET_LINE_MASK="(1 << 7)"
CC_FLAGS += -DAVR_ERASE_LINE_PORT="PORTC"
CC_FLAGS += -DAVR_ERASE_LINE_DDR="DDRC"
CC_FLAGS += -DAVR_ERASE_LINE_MASK="(1 << 6)"

# Optional Variables 
AVRDUDE_PROGRAMMER = avrisp2
ARDUINO_MODEL_PID = 0x0010

# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk
