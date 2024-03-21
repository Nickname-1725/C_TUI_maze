build/foo: main.c
	gcc -Wall -o build/foo main.c -lncursesw

clean:
	rm -f build/*.o
	rm build/foo
