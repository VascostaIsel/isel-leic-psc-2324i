#include "dictionary.h"
#include <stdio.h>

Dictionary *dictionary_create() {
    Dictionary *dictionary = g_new(Dictionary, 1);
    dictionary->words = g_hash_table_new(g_str_hash, g_str_equal);
    return dictionary;
}

void dictionary_add(Dictionary *dictionary, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (dictionary_lookup(dictionary, line) == 0) {
        	g_hash_table_add(dictionary->words, g_strdup(line));
        }       
    }
    fclose(file);
}

int dictionary_lookup(Dictionary *dictionary, const char *word) {
	return 0;
}
