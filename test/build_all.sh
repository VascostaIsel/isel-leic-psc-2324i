
# dictionary_create_test


# dictionary_add_test
gcc -g -Wall dictionary_add_test.c ../dictionary.c ../invoke/invoke.s -o dictionary_add_test -L../bin -lDictionary -I../bin `pkg-config --cflags --libs glib-2.0`

# dictionary_lookup_test
gcc -g -Wall dictionary_lookup_test.c ../dictionary.c ../invoke/invoke.s -o dictionary_lookup_test -L../bin -lDictionary -I../bin `pkg-config --cflags --libs glib-2.0`

# dictionary_destroy_test

