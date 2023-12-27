gcc -g -Wall spell_check.c ../dictionary.c -o spell_check -L../bin -lDictionary -I../bin `pkg-config --cflags --libs glib-2.0`
