# Compiler and flags
CC = gcc
CFLAGS = -pedantic -ggdb -Wall -Wextra -Werror 
# Name of the executable
NAME = SLIME
# Files
SRC = src/main.c
OBJ = $(SRC:.c=.o)
# Rules
sdl:
	cd lib/SDL && mkdir build && cd build && ../configure && make && sudo make install

sdl-image:
	cd lib/SDL_Image && mkdir build && cd build && ../configure && make && sudo make install

sdl-ttf:
	cd lib/SDL_ttf && mkdir build && cd build && ../configure && make && sudo make install

libs:
	$(MAKE) sdl
	$(MAKE) sdl-image
	$(MAKE) sdl-ttf

setup:
	mkdir obj
	mkdir bin
	mkdir obj/engine
	mkdir obj/game
	mkdir obj/math
	mkdir obj/other
	mkdir obj/menus

main:
	$(CC) $(CFLAGS) -c src/math/collider.c -o obj/math/collider.o
	$(CC) $(CFLAGS) -c src/other/linkedlist.c -o obj/other/linkedlist.o
	$(CC) $(CFLAGS) -c src/engine/key_manager.c -o obj/engine/key_manager.o
	$(CC) $(CFLAGS) -c src/engine/engine.c -o obj/engine/engine.o
	$(CC) $(CFLAGS) -c src/menus/text_renderer.c -o obj/menus/text_renderer.o
	$(CC) $(CFLAGS) -c src/menus/animation.c -o obj/menus/animation.o
	$(CC) $(CFLAGS) -c src/menus/item.c -o obj/menus/item.o
	$(CC) $(CFLAGS) -c src/menus/image.c -o obj/menus/image.o
	$(CC) $(CFLAGS) -c src/menus/menu.c -o obj/menus/menu.o
	$(CC) $(CFLAGS) -c src/menus/text_renderer.c -o obj/menus/text_renderer.o
	$(CC) $(CFLAGS) -c src/game/texture.c -o obj/game/texture.o
	$(CC) $(CFLAGS) -c src/game/player_state.c -o obj/game/player_state.o
	$(CC) $(CFLAGS) -c src/game/slime.c -o obj/game/slime.o
	$(CC) $(CFLAGS) -c src/game/ball.c -o obj/game/ball.o
	$(CC) $(CFLAGS) -c src/game/rect_collide.c -o obj/game/rect_collide.o
	$(CC) $(CFLAGS) -c src/game/game.c -o obj/game/game.o
	$(CC) $(CFLAGS) -o bin/$(NAME) $(SRC) obj/math/collider.o obj/other/linkedlist.o obj/engine/key_manager.o obj/engine/engine.o obj/menus/text_renderer.o obj/menus/animation.o obj/menus/item.o obj/menus/image.o obj/menus/menu.o obj/game/texture.o obj/game/player_state.o obj/game/slime.o obj/game/ball.o obj/game/rect_collide.o obj/game/game.o `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm


remap:
	$(CC) -o bin/REMAP remap/remap_gpio.c -lgpiod -ludev -lpthread

clean:
	rm -Rf bin
	rm -Rf obj
