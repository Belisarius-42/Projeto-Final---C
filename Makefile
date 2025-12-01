# Makefile para Sistema de Gestão de Inventário

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = inventario
SOURCES = main.c inventario.c
OBJECTS = $(SOURCES:.c=.o)

# Compilação padrão
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Regra para compilar arquivos objeto
%.o: %.c inventario.h
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos compilados
clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe

# Limpar também o arquivo de dados
clean-all: clean
	rm -f inventario.dat

# Executar o programa
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean clean-all run


