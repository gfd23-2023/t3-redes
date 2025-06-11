# Compilador e flags
CC       = gcc
CFLAGS   = -Wall -Wextra -g -O0 -fsanitize=address -fno-omit-frame-pointer
LDFLAGS  = -fsanitize=address

# Nome do executável principal
MAIN     = Hamming

# Fontes
SRC      = Hamming.c hamming31_26.c
OBJS     = Hamming.o hamming31_26.o

.PHONY: all clean run

# Compilação padrão
all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Executar o programa (opcional)
run: $(MAIN)
	./$(MAIN)

# Limpeza
clean:
	rm -f $(MAIN) *.o *~ temp.txt saida.txt entrada.txt

