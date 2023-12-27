#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <./bin/dictionary.h>
#include <dictionary_proh.h>

#define MAX_WORD_SIZE 32
#define SIZE_INCREMENT 64   //  Bytes, needs adjustment for structs

//  Needs size calculation to adjust increment
//  Would theoretically be 4 + 4 + 1 = 9 bytes
//  So to store 64 more structs we would need 64 * 9 = 576 bytes

typedef struct {
    int row;
    int col;
} Pair;

int main(int argc, char *argv[]){
    int is_file = 0;
    int is_word = 0;

    char * text_to_check[128];

    GList * dictionary_name_list;
    char * word[32];
    
    for (int i = 1; i < argc; ++i) {
        if (is_file && is_word){
            perror("An error occurred: Cannot have both -t and -w arguments. Please use one or the other.\n");
            return(-1);
        }
        else{
            //  Done
            if(strcmp(argv[i], "-t") == 0){
                printf("%s\n", argv[i+1]);
                text_to_check = argv[i+1];
                is_file = 1;

            //  Done
            }else if(strcmp(argv[i], "-w") == 0){
                printf("%s\n", argv[i+1]);
                word = argv[i+1];
                is_word = 1;

            //  Done
            }else if(strcmp(argv[i], "-d") == 0){
                printf("%s\n", argv[i+1]);
                dictionary_name_list = g_list_append(dictionary_name_list, g_strdup(argv[i+1]));
            }
        }
    }
    
    //  Done
    if (isFile) {
        GHashTable * missing_words;
        for (dictionary = dictionary_name_list; dictionary != NULL; dictionary = g_list_next(dictionary_name_list)) {
            missing_words = check_if_file_in_dictionary(text_to_check, dictionary, missing_words)
        }
        if (g_hash_table_size(missing_words) == 0) {
            printf("Every word of the File %s is in all of the dictionaries\n", text_to_check);
        } else {
            printf("The following words are not in the dictionaries:\n");
            g_hash_table_foreach(missing_words, print_word, NULL);
        }
        g_hash_table_destroy(missing_words);
    } else {
        int is_word_in_dictionary = 0;
        for (elem = dictionary_name_list; elem; elem = g_list_next(dictionary_name_list)) {
            is_word_in_dictionary += _dictionary(word, dictionary_name_elem->data);
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

void print_word(gpointer key, gpointer value, gpointer user_data) {
    const gchar *word = (const gchar *)key;
    Pair *pair = (Pair *)value;
    
    printf("%s - %d/%d\n", word, pair->row, pair->col);
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

//  GHashTable<String, Pair(row, col)> missing_words
GHashTable *check_if_file_in_dictionary(const char *file_to_check, const char *dictionary_file_contributor, GHashTable *missing_words) {

    FILE *file_ptr;
    file_ptr = fopen(file_to_check, "r");
    if (file_ptr == NULL) {
        printf("\nFailed to open file: %s\n", file_to_check);
        return NULL;
    }

    //  Each word limited to 32 chars
    char word[MAX_WORD_SIZE];

    //  Creates dictionary based on param file
    Dictionary * dic = dictionary_create();
    dictionary_add(dic, dictionary_file_contributor);

    int row = 0;
    int col = 0;
    Pair coords = {0, 0};

    //  Static 32 chars but could be changed
    while (fscanf(file_ptr, "%32s", word) == 1) {
        if (strcmp(word, "\n") == 0) {
            row += 1;
            col = 0;
        } else {
            if (dictionary_lookup(dic->words, word) == 0) {
                coords->row = row;
                coords->col = col;
                dictionary_add(missing_words, coords);
            }
            col += 1;
        }
    }

    //  Frees memory and closes file
    fclose(file_ptr);
    dictionary_destroy(dic);

    return missingWords;
}