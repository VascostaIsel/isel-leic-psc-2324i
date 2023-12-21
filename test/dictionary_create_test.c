#include <stdio.h>
#include "dictionary.h"

const char *register_name[] = { "", "rbp", "rbx", "r15", "r14", "r13", "r12" };

int invoke_and_test(void *, void (*)(), int, ...);

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

int compare_hash_tables(GHashTable *table1, GHashTable *table2) {
	if (table1 == NULL && table2 != NULL) return 1;
	if (table1 != NULL && table2 == NULL) return 1;
    if (g_hash_table_size(table1) != g_hash_table_size(table2))	return 1;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, table1);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        if (!g_hash_table_contains(table2, key)) return 1;

        const char *value1 = (const char *)value;
        const char *value2 = (const char *)g_hash_table_lookup(table2, key);

        if (g_strcmp0(value1, value2) != 0) return 1;
    }
    return 0;
}

struct {
    Dictionary *dictionary;
    int expected_result;
} 
test_array[] = {
    { .dictionary = NULL, .expected_result = 0 },
    { .dictionary = NULL, .expected_result = 1},
    { .dictionary = NULL, .expected_result = 1 },
};

Dictionary *dictionary_create();

int main() {
    GHashTable *words = g_hash_table_new(g_str_hash,g_str_equal);
    g_hash_table_add(words,"test");
    
    GHashTable *words2 = g_hash_table_new(g_str_hash, g_str_equal);
	g_hash_table_add(words2, "test1");

    test_array[0].dictionary = &(Dictionary){ .words = words };
    test_array[1].dictionary = &(Dictionary){ .words = words2 };
    test_array[2].dictionary = &(Dictionary){ .words = NULL };
    
    
		
    for (size_t i = 0; i < ARRAY_SIZE(test_array); i++) {
    	Dictionary *test_dictionary;
        int result = invoke_and_test(&test_dictionary, (void (*)())dictionary_create, 0);
        if (result != 0) {
			printf("Your function currupted %s, that is a calee saved register\n", register_name[result]);
			break;
		}

        g_hash_table_add(test_dictionary->words,"test");
        
        int compare_result = compare_hash_tables(test_dictionary->words, test_array[i].dictionary->words);
        if (compare_result != test_array[i].expected_result) 
			printf("[%zd] Failed - Received: %d Expected: %d\n", i, compare_result, test_array[i].expected_result);
		else 
			printf("[%zd] Passed - Received: %d Expected: %d\n", i, compare_result, test_array[i].expected_result);
        
        g_hash_table_destroy(test_dictionary->words);
    }
    g_hash_table_destroy(words);
}
