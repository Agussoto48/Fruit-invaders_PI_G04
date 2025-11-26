# ============================================
# CONFIGURACIÓN DEL PROYECTO - FRUIT INVADERS
# ============================================

# --- PC (Juego Principal) ---
CXX_PC      := g++
CXXFLAGS_PC := -std=c++17 -O2 -Wno-unused-variable
NASM        := nasm
NASMFLAGS   := -f elf64

# Archivos del juego
GAME_CPP_SRCS := src/cpp/main.cpp src/cpp/combate.cpp src/cpp/player.cpp \
                 src/cpp/enemigo.cpp src/cpp/disparo.cpp src/cpp/obstacle.cpp \
                 src/cpp/block.cpp
GAME_ASM_SRCS := src/ensamblador/movJugador.asm src/ensamblador/movEnemigo.asm \
                 src/ensamblador/blockDrawAsm.asm src/ensamblador/blockGetRectAsm.asm \
                 src/ensamblador/obstacleEmitPosition.asm
GAME_OBJS     := $(GAME_CPP_SRCS:.cpp=.o) $(GAME_ASM_SRCS:.asm=.o)
GAME_BIN      := bin/fruit_invaders

# Librerías de Raylib
LIBS_PC := -lraylib -lm -ldl -lpthread -lGL -lGLU -lX11

# --- Arduino / AVR (Joystick) ---
ARDUINO_DIR ?= $(PWD)/third_party/ArduinoCore-avr

MCU   := atmega328p
F_CPU := 16000000UL
PORT  := /dev/ttyACM0
BAUD  := 115200

SKETCH  := arduino_joystick.cc

AVR_CC      := avr-gcc
AVR_CXX     := avr-g++
AVR_OBJCOPY := avr-objcopy
AVR_SIZE    := avr-size

AVR_INCLUDES := -I$(ARDUINO_DIR)/cores/arduino -I$(ARDUINO_DIR)/variants/standard
AVR_DEFS     := -DF_CPU=$(F_CPU) -DARDUINO=10819 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR
AVR_CFLAGS   := -mmcu=$(MCU) -Os -ffunction-sections -fdata-sections -MMD -flto $(AVR_DEFS) $(AVR_INCLUDES)
AVR_CXXFLAGS := $(AVR_CFLAGS) -fno-exceptions -std=gnu++11
AVR_LDFLAGS  := -mmcu=$(MCU) -Os -Wl,--gc-sections -flto

ARDUINO_BUILD := arduino_build

# Fuentes del core Arduino
CORE_C_SRCS   := $(wildcard $(ARDUINO_DIR)/cores/arduino/*.c)
CORE_CPP_SRCS := $(wildcard $(ARDUINO_DIR)/cores/arduino/*.cpp)
VARIANT_SRCS  := $(wildcard $(ARDUINO_DIR)/variants/standard/*.c) \
                 $(wildcard $(ARDUINO_DIR)/variants/standard/*.cpp)

# Todos los objetos Arduino
ARDUINO_OBJS  := \
  $(patsubst $(ARDUINO_DIR)/cores/arduino/%.c,$(ARDUINO_BUILD)/core/%.o,$(CORE_C_SRCS)) \
  $(patsubst $(ARDUINO_DIR)/cores/arduino/%.cpp,$(ARDUINO_BUILD)/core/%.o,$(CORE_CPP_SRCS)) \
  $(patsubst $(ARDUINO_DIR)/variants/standard/%.c,$(ARDUINO_BUILD)/variant/%.o,$(filter %.c,$(VARIANT_SRCS))) \
  $(patsubst $(ARDUINO_DIR)/variants/standard/%.cpp,$(ARDUINO_BUILD)/variant/%.o,$(filter %.cpp,$(VARIANT_SRCS))) \
  $(ARDUINO_BUILD)/sketch.o

ELF := $(ARDUINO_BUILD)/firmware.elf
HEX := $(ARDUINO_BUILD)/firmware.hex

# ============================================
# OBJETIVOS PRINCIPALES
# ============================================
all: game

game: $(GAME_BIN)

arduino: $(HEX)

upload: $(HEX)
	avrdude -v -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -D -U flash:w:$(HEX):i

full: arduino upload game

# ============================================
# JUEGO PRINCIPAL (PC)
# ============================================
$(GAME_BIN): $(GAME_OBJS)
	@mkdir -p bin
	$(CXX_PC) $(CXXFLAGS_PC) $^ -o $@ $(LIBS_PC) -no-pie

src/cpp/%.o: src/cpp/%.cpp
	@mkdir -p $(dir $@)
	$(CXX_PC) $(CXXFLAGS_PC) -c $< -o $@

src/ensamblador/%.o: src/ensamblador/%.asm
	@mkdir -p $(dir $@)
	$(NASM) $(NASMFLAGS) $< -o $@

# ============================================
# ARDUINO (JOYSTICK)
# ============================================
$(ARDUINO_BUILD)/sketch.o: $(SKETCH)
	@mkdir -p $(ARDUINO_BUILD)
	$(AVR_CXX) $(AVR_CXXFLAGS) -c $< -o $@

$(ARDUINO_BUILD)/core/%.o: $(ARDUINO_DIR)/cores/arduino/%.c
	@mkdir -p $(ARDUINO_BUILD)/core
	$(AVR_CC) $(AVR_CFLAGS) -c $< -o $@

$(ARDUINO_BUILD)/core/%.o: $(ARDUINO_DIR)/cores/arduino/%.cpp
	@mkdir -p $(ARDUINO_BUILD)/core
	$(AVR_CXX) $(AVR_CXXFLAGS) -c $< -o $@

$(ARDUINO_BUILD)/variant/%.o: $(ARDUINO_DIR)/variants/standard/%.c
	@mkdir -p $(ARDUINO_BUILD)/variant
	$(AVR_CC) $(AVR_CFLAGS) -c $< -o $@

$(ARDUINO_BUILD)/variant/%.o: $(ARDUINO_DIR)/variants/standard/%.cpp
	@mkdir -p $(ARDUINO_BUILD)/variant
	$(AVR_CXX) $(AVR_CXXFLAGS) -c $< -o $@

$(ELF): $(ARDUINO_OBJS)
	$(AVR_CXX) $(AVR_LDFLAGS) $^ -o $@
	$(AVR_SIZE) $@

$(HEX): $(ELF)
	$(AVR_OBJCOPY) -O ihex -R .eeprom $< $@

# ============================================
# LIMPIAR
# ============================================
clean:
	$(RM) -r src/cpp/*.o src/ensamblador/*.o bin/
	$(RM) -r $(ARDUINO_BUILD)

clean_arduino:
	$(RM) -r $(ARDUINO_BUILD)

.PHONY: all game arduino upload full clean clean_arduino