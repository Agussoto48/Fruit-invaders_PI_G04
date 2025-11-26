CXX_PC      := g++
CXXFLAGS_PC := -std=c++17 -O2 -Wno-unused-variable
NASM        := nasm
NASMFLAGS   := -f elf64


BIN_DIR := bin

GAME_CPP_SRCS := src/cpp/main.cpp src/cpp/combate.cpp src/cpp/player.cpp \
                 src/cpp/enemigo.cpp src/cpp/disparo.cpp src/cpp/obstacle.cpp \
                 src/cpp/block.cpp src/cpp/menuInicio.cpp src/cpp/musica.cpp \
                 src/cpp/pausado.cpp src/cpp/gameOver.cpp

GAME_ASM_SRCS := src/ensamblador/movJugador.asm src/ensamblador/movEnemigo.asm \
                 src/ensamblador/blockDrawAsm.asm src/ensamblador/blockGetRectAsm.asm \
                 src/ensamblador/obstacleEmitPosition.asm src/ensamblador/actualizarDisparo.asm \
                 src/ensamblador/calcularPuntaje.asm

# OBJETOS AHORA VAN A bin/
GAME_OBJS     := $(patsubst src/cpp/%.cpp,$(BIN_DIR)/%.o,$(GAME_CPP_SRCS)) \
                 $(patsubst src/ensamblador/%.asm,$(BIN_DIR)/%.o,$(GAME_ASM_SRCS))

GAME_BIN      := $(BIN_DIR)/fruit_invaders

LIBS_PC := -lraylib -lm -ldl -lpthread -lGL -lGLU -lX11

ARDUINO_DIR ?= $(PWD)/thirdParty/ArduinoCore-avr

MCU   := atmega328p
F_CPU := 16000000UL
PORT  := /dev/ttyACM0
BAUD  := 115200

SKETCH  := arduino_joystick.cc

AVR_CC      := avr-gcc
AVR_CXX     := avr-g++
AVR_OBJCOPY := avr-objcopy
AVR_SIZE    := avr-size

ARDUINO_BUILD := $(BIN_DIR)/arduino_build

AVR_INCLUDES := -I$(ARDUINO_DIR)/cores/arduino -I$(ARDUINO_DIR)/variants/standard
AVR_DEFS     := -DF_CPU=$(F_CPU) -DARDUINO=10819 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR
AVR_CFLAGS   := -mmcu=$(MCU) -Os -ffunction-sections -fdata-sections -MMD -flto $(AVR_DEFS) $(AVR_INCLUDES)
AVR_CXXFLAGS := $(AVR_CFLAGS) -fno-exceptions -std=gnu++11
AVR_LDFLAGS  := -mmcu=$(MCU) -Os -Wl,--gc-sections -flto

CORE_C_SRCS   := $(wildcard $(ARDUINO_DIR)/cores/arduino/*.c)
CORE_CPP_SRCS := $(wildcard $(ARDUINO_DIR)/cores/arduino/*.cpp)
VARIANT_SRCS  := $(wildcard $(ARDUINO_DIR)/variants/standard/*.c) \
                 $(wildcard $(ARDUINO_DIR)/variants/standard/*.cpp)

ARDUINO_OBJS  := \
  $(patsubst $(ARDUINO_DIR)/cores/arduino/%.c,$(ARDUINO_BUILD)/core/%.o,$(CORE_C_SRCS)) \
  $(patsubst $(ARDUINO_DIR)/cores/arduino/%.cpp,$(ARDUINO_BUILD)/core/%.o,$(CORE_CPP_SRCS)) \
  $(patsubst $(ARDUINO_DIR)/variants/standard/%.c,$(ARDUINO_BUILD)/variant/%.o,$(filter %.c,$(VARIANT_SRCS))) \
  $(patsubst $(ARDUINO_DIR)/variants/standard/%.cpp,$(ARDUINO_BUILD)/variant/%.o,$(filter %.cpp,$(VARIANT_SRCS))) \
  $(ARDUINO_BUILD)/sketch.o

ELF := $(ARDUINO_BUILD)/firmware.elf
HEX := $(ARDUINO_BUILD)/firmware.hex

all: game

game: $(GAME_BIN)

arduino: $(HEX)

upload: $(HEX)
	avrdude -v -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -D -U flash:w:$(HEX):i

full: arduino upload game

$(GAME_BIN): $(GAME_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX_PC) $(CXXFLAGS_PC) $^ -o $@ $(LIBS_PC) -no-pie

# COMPILAR C++ HACIA bin/
$(BIN_DIR)/%.o: src/cpp/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX_PC) $(CXXFLAGS_PC) -c $< -o $@

# COMPILAR ASM HACIA bin/
$(BIN_DIR)/%.o: src/ensamblador/%.asm
	@mkdir -p $(BIN_DIR)
	$(NASM) $(NASMFLAGS) $< -o $@

# COMPILAR SKETCH HACIA bin/arduino_build
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

clean:
	rm -rf $(BIN_DIR)

clean_arduino:
	rm -rf $(ARDUINO_BUILD)

run:
	./$(GAME_BIN)

.PHONY: all game arduino upload full clean clean_arduino
