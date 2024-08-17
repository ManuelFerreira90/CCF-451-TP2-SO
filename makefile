# Nome do compilador
CC = gcc

# Opções de compilação
CFLAGS = -O2 -w  # -w suprime todos os avisos

# Diretórios
SRC_DIR = src
OBJ_DIR = obj

# Arquivos fontes e objetos
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Nome do executável
EXEC = exe

# Regra padrão
all: $(EXEC)

# Regra para compilar o executável
$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para compilar arquivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Regra para limpar arquivos temporários
clean:
	rm -f $(EXEC) $(OBJECTS)
	rm -rf $(OBJ_DIR)

# Regra para executar o programa
run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
