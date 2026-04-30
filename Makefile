CC = gcc
FLAGS = -I./include
LIBS = -lraylib -lm -lpthread -ldl -lX11

all:
	$(CC) ./src/*.c $(FLAGS) $(LIBS) -o jogo

clean:
	rm -f jogo
