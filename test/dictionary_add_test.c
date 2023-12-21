#include <stdio.h>
#include "dictionary.h"

const char *register_name[] = { "", "rbp", "rbx", "r15", "r14", "r13", "r12" };

int invoke_and_test(void *, void (*)(), int, ...);

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

void print_hash_table(GHashTable *hash_table) {	
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, hash_table);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        printf("%s ", (char *)key);
    }
}

int compare_hash_tables(GHashTable *table1, GHashTable *table2) {
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
    const char *fileName;
    GHashTable *expected_result;
} test_array[] = {
    { .fileName = "../test/small_input.txt", .expected_result = NULL },
    { .fileName = NULL, .expected_result = NULL },
};

void dictionary_add(Dictionary *dictionary, const char *filename);

int main() {
	GHashTable *words = g_hash_table_new(g_str_hash, g_str_equal);
	g_hash_table_add(words, "test");
	g_hash_table_add(words, "test1");
	g_hash_table_add(words, "test2");
	
	GHashTable *words2 = g_hash_table_new(g_str_hash, g_str_equal);
    
    test_array[0].expected_result = words;
    test_array[1].expected_result = words2;

	for (size_t i = 0; i < ARRAY_SIZE(test_array); i++) {
		GHashTable *test_words = g_hash_table_new(g_str_hash, g_str_equal);
		Dictionary *test_dictionary = &(Dictionary){ .words = test_words };
		
		int received_result;
		int result = invoke_and_test(&received_result, (void (*)())dictionary_add, 2, test_dictionary, test_array[i].fileName);
		if (result != 0) {
			printf("Your function currupted %s, that is a calee saved register\n", register_name[result]);
			break;
		}
		if (compare_hash_tables(test_dictionary->words, test_array[i].expected_result) == 1) {
			printf("[%zd] Failed\n", i);
			printf("Received: ");
            print_hash_table(test_dictionary->words);
            printf("\nExpected: ");
            print_hash_table(test_array[i].expected_result);
		}
		else {
			printf("[%zd] Passed\n", i);
			printf("Received: ");
            print_hash_table(test_dictionary->words);
            printf("\nExpected: ");
            print_hash_table(test_array[i].expected_result);
		}
		printf("\n");
	}
	g_hash_table_destroy(words);
}
