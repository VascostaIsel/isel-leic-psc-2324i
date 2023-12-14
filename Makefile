.PHONY: static dynamic clean

dynamic: bin/dictionaryLib.so

static: bin/dictionaryLib.a

bin/dictionaryLib.so: dictionary.o
	gcc -shared -g -o $@ dictionary.o
	
bin/dictionaryLib.a: dictionary.o
	ar rcs $@ dictionary.o

dictionary.o: dictionary.c bin/dictionary.h
	gcc -Wall -pedantic -g -fPIC -Ibin -c -o $@ dictionary.c `pkg-config --cflags --libs glib-2.0`

clean:
	rm -f dictionary.o
	rm -f bin/dictionaryLib.so
	rm -f bin/dictionaryLib.a
