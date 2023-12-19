.PHONY: static dynamic clean

dynamic: bin/libDictionary.so

static: bin/libDictionary.a

bin/libDictionary.so: dictionary.o
	gcc -shared -g -o $@ dictionary.o
	
bin/libDictionary.a: dictionary.o
	ar rcs $@ dictionary.o

dictionary.o: dictionary.c bin/dictionary.h
	gcc -Wall -pedantic -g -fPIC -Ibin -c -o $@ dictionary.c `pkg-config --cflags --libs glib-2.0`

clean:
	rm -f dictionary.o
	rm -f bin/libDictionary.so
	rm -f bin/libDictionary.a
