CFLAGS=-O0 -g -Wall -rdynamic `pkg-config --cflags glib-2.0`
LIBS=-Llib -lm -lpthread -lglib-2.0 -lglfw -lGL -ldl -lfreetype -lglad #-lassimp libSTB_IMAGE.a 

all: clean pong

pong:
	$(CC) $(CFLAGS) $(LIBS) -o pong pong.c glad.c

clean:
	rm -f pong
