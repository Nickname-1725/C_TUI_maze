build/foo: main.c
	gcc -Wall -o build/foo main.c -lncursesw

build/test: maze_gen.c
	gcc -o build/test maze_gen.c
build/test: maze_gen.h

clean:
	rm -f build/*.o
	rm build/foo
