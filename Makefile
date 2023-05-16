INCLUDES = -I./lib/headers -I./src/util
FLAGS = -Wall -fPIC -g $(INCLUDES)
LIBFLAGS = -L./lib/binaries
LDFLAGS = -lGL -lglfw3
CFLAGS = -std=c99
CPPFLAGS = -std=c++0x
CC = g++

.PHONY: clean

main: obj/main.o obj/glad.o 
	$(CC) $(FLAGS) $(LIBFLAGS) -o $@ $^ $(LDFLAGS)

obj/main.o: obj/shader_constants.h main.c
	$(CC) $(FLAGS) $(INCLUDES) -o $@ -c $(CFLAGS) $^ 

obj/shader_constants.h: shaders/vertex.glsl shaders/fragment.glsl
	bash -c "xxd -i shaders/vertex.glsl | tac | sed '3s/$$/, 0x00/' | tac > obj/shader_constants.h"
	bash -c "xxd -i shaders/fragment.glsl | tac | sed '3s/$$/, 0x00/' | tac >> obj/shader_constants.h"

obj/glad.o: obj/shader_constants.h lib/headers/glad.c
	$(CC) $(FLAGS) $(INCLUDES) -c $(CFLAGS) $^  -o $@

clean:
	rm -f obj/main.o obj/glad.o obj/main main obj/shader_constants.h
