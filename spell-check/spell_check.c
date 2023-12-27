#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dictionary.h"

#define MAX_WORD_SIZE 32

typedef struct {
    int row;
    int col;
} Pair;

int main(int argc, char *argv[]){
    int is_file = 0;
    int is_word = 0;

    GList * dictionary_name_list;
    char *text_to_check;
    char *word;

    int opt;
    while ((opt = getopt(argc, argv, "t:w:d:")) != -1) {
    	if (is_file && is_word){
            perror("An error occurred: Cannot have both -t and -w arguments. Please use one or the other.\n");
            return(-1);
        }
        switch (opt) {
            case 't':
            	text_to_check = optarg;
                is_file = 1;
                break;
            case 'w':
                word = optarg;
                is_word = 1;
                break;
            case 'd':
                dictionary_name_list = g_list_append(dictionary_name_list, g_strdup(optarg));
                break;             
            default:
                fprintf(stderr, "usage: ./spell_check <option>");
                exit(EXIT_FAILURE);
        }
    }
    
    if (is_file == 1) {
        GHashTable *missing_words = g_hash_table_new(g_str_hash, g_str_equal);
        for (GList *dict_elem = dictionary_name_list; dict_elem != NULL; dict_elem = g_list_next(dict_elem)) {
    		check_if_file_in_dictionary(text_to_check, dict_elem->data, missing_words);
		}
        if (g_hash_table_size(missing_words) == 0) {
            printf("Every word of the File %s is in all of the dictionaries\n", text_to_check);
        } else {
            printf("The following words are not in the dictionaries:\n");
            print_hash_table(missing_words);
        }
        g_hash_table_destroy(missing_words);
    } else {
        int is_word_in_dictionary = 0;
        for (GList *elem = dictionary_name_list; elem != NULL; elem = g_list_next(elem)) {
			is_word_in_dictionary += check_if_word_in_dictionary(word, elem->data);
		}
        if (is_word_in_dictionary == 0) {
            printf("Word %s is not in any of the dictionaries\n", word);
        } else {
            printf("Word %s is in at least one of the dictionaries\n", word);
        }
    }
    g_list_free(dictionary_name_list);

    return 0;
}

void print_hash_table(GHashTable *hash_table) {	
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, hash_table);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
    	Pair *pair = (Pair *)value;
        printf("%s - %d/%d\n", (char *)key, pair->row, pair->col);
    }
}

int check_if_word_in_dictionary(const char *word_to_check, const char *dictionary_file_contributor) {
    //  Creates dictionary based on param file
    Dictionary * dic = dictionary_create();
    dictionary_add(dic, dictionary_file_contributor);

    int result = dictionary_lookup(dic, word_to_check);

    //  Frees memory
    dictionary_destroy(dic);

    return result;
}

// GHashTable<String, Pair(row, col)> missing_words
void check_if_file_in_dictionary(const char *file_to_check, const char *dictionary_file_contributor, GHashTable *missing_words) {

    FILE *file_ptr;
    file_ptr = fopen(file_to_check, "r");
    if (file_ptr == NULL) {
        printf("\nFailed to open file: %s\n", file_to_check);
        return;
    }

    //  Each word limited to 32 chars
    char word[MAX_WORD_SIZE];

    //  Creates dictionary based on param file
    Dictionary * dic = dictionary_create();
    dictionary_add(dic, dictionary_file_contributor);

    int row = 1;

    //  Static 32 chars but could be changed
    while (fscanf(file_ptr, "%32[^\n]%*c", word) == 1) {
    	int col = 1;
    	const char *delimiter = " \t";
    	
    	char *token = strtok(word, delimiter);
    	while (token != NULL) {
        	if (dictionary_lookup(dic, token) == 0) {
            	char *word_key = g_strdup(token);
	
            	Pair *coords = g_malloc(sizeof(Pair));
            	coords->row = row;
            	coords->col = col;
            	g_hash_table_insert(missing_words, word_key, coords);
        	}
        		col += 1;
        		token = strtok(NULL, delimiter);
    		}
	
    		row += 1;
	}

    //  Frees memory and closes file
    fclose(file_ptr);
    dictionary_destroy(dic);
}
