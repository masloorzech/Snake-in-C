CC = gcc
CFLAGS = -g -pedantic -Wall -pthread -DGL_SILENCE_DEPRECATION   # Dodatkowe flagi kompilatora, w tym -I dla FreeType
LDFLAGS = -lglfw -framework OpenGL  # Biblioteki do linkowania

app: app.c snake.c
	$(CC) $(CFLAGS) -o app app.c snake.c assets_loader.c $(LDFLAGS)
