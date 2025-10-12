CXX = g++
ASM = nasm

CXXFLAGS = -Wall -std=c++17 -O2
ASMFLAGS = -f elf64

# Librerías de Raylib
LIBS = -lraylib -lm -ldl -lpthread -lGL -lGLU -lX11

# Carpetas
BIN_DIR = bin
CPP_DIR = src/cpp
ASM_DIR = src/ensamblador

# Archivos
SRC_CPP = $(wildcard $(CPP_DIR)/*.cpp)
SRC_ASM = $(wildcard $(ASM_DIR)/*.asm)

# Archivos objeto
OBJ_CPP = $(patsubst $(CPP_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRC_CPP))
OBJ_ASM = $(patsubst $(ASM_DIR)/%.asm,$(BIN_DIR)/%.o,$(SRC_ASM))

# Ejecutable
EXE = $(BIN_DIR)/programa

# Regla principal
default: build

build: $(EXE)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(EXE): $(OBJ_ASM) $(OBJ_CPP)
	$(CXX) $(OBJ_ASM) $(OBJ_CPP) -o $(EXE) $(LIBS)

# Compilar C++
$(BIN_DIR)/%.o: $(CPP_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar ASM
$(BIN_DIR)/%.o: $(ASM_DIR)/%.asm | $(BIN_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

# Ejecutar
run: $(EXE)
	@./$(EXE)

clean:
	@rm -rf $(BIN_DIR)
