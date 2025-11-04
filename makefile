# ===== Herramientas =====
CXX = g++
ASM = nasm

# ===== Carpetas =====
BIN_DIR = bin
CPP_DIR = src/cpp
ASM_DIR = src/ensamblador
INC_DIR = include

# ===== Fuentes =====
SRC_CPP := $(wildcard $(CPP_DIR)/*.cpp)
SRC_ASM := $(wildcard $(ASM_DIR)/*.asm) $(wildcard $(ASM_DIR)/*.ASM)

# Objetos (prefijo para evitar colisiones)
OBJ_CPP := $(patsubst $(CPP_DIR)/%.cpp,$(BIN_DIR)/cpp_%.o,$(SRC_CPP))
OBJ_ASM := $(patsubst $(ASM_DIR)/%.asm,$(BIN_DIR)/asm_%.o,$(filter %.asm,$(SRC_ASM))) \
           $(patsubst $(ASM_DIR)/%.ASM,$(BIN_DIR)/asm_%.o,$(filter %.ASM,$(SRC_ASM)))

# Ejecutable
EXE = $(BIN_DIR)/programa

# ===== Flags (solo 64-bit) =====
CXXFLAGS = -Wall -std=c++17 -O2 -I$(INC_DIR) -MMD -MP -m64
ASMFLAGS = -f elf64
LDFLAGS  = -m64

# ===== Raylib por pkg-config si está, si no fallback =====
PKG_RL_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)
PKG_RL_LIBS   := $(shell pkg-config --libs raylib 2>/dev/null)

ifeq ($(strip $(PKG_RL_LIBS)),)
  # Fallback Linux/X11
  LIBS = -lraylib -lm -ldl -lpthread -lGL -lX11 -lrt
else
  CXXFLAGS += $(PKG_RL_CFLAGS)
  LIBS = $(PKG_RL_LIBS)
endif

# ===== Reglas =====
.PHONY: default build run clean debug release list print-%

default: build
build: $(EXE)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Link (solo objetos .o)
$(EXE): $(OBJ_ASM) $(OBJ_CPP) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

# C++
$(BIN_DIR)/cpp_%.o: $(CPP_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ASM (NASM) — .asm y .ASM
$(BIN_DIR)/asm_%.o: $(ASM_DIR)/%.asm | $(BIN_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

$(BIN_DIR)/asm_%.o: $(ASM_DIR)/%.ASM | $(BIN_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

run: $(EXE)
	./$(EXE)

clean:
	rm -rf $(BIN_DIR)

debug: CXXFLAGS += -g -O0 -DDEBUG
debug: clean build

release: CXXFLAGS += -O3 -DNDEBUG
release: clean build

# Ver qué archivos está tomando
list:
	@echo "CPP: $(SRC_CPP)"
	@echo "ASM: $(SRC_ASM)"
	@echo "OBJ_CPP: $(OBJ_CPP)"
	@echo "OBJ_ASM: $(OBJ_ASM)"

# Imprimir variables: make print-NOMBRE
print-%:
	@echo '$* = $($*)'

# Incluir dependencias de C++ si existen
-include $(OBJ_CPP:.o=.d)
