#include <stdio.h>
#include "dictionary.h"

const char *register_name[] = { "", "rbp", "rbx", "r15", "r14", "r13", "r12" };

int invoke_and_test(void *, void (*)(), int, ...);

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

struct {
    Dictionary *dictionary;
    const char *word;
    int expected_result;
} test_array[] = {
    { .dictionary = NULL, .word = "test", .expected_result = 0 },
    { .dictionary = NULL, .word = "test1", .expected_result = 1 },
    { .dictionary = NULL, .word = NULL, .expected_result = 1 },
};

int dictionary_lookup(Dictionary *dictionary, const char *word);

int main() {
	GHashTable *words = g_hash_table_new(g_str_hash, g_str_equal);
	g_hash_table_add(words, "test");
	
	test_array[0].dictionary = &(Dictionary){ .words = words };
    test_array[1].dictionary = &(Dictionary){ .words = words };
    test_array[2].dictionary = &(Dictionary){ .words = words };

	for (size_t i = 0; i < ARRAY_SIZE(test_array); i++) {
		int received_result;
		int result = invoke_and_test(&received_result, (void (*)())dictionary_lookup, 2, test_array[i].dictionary, test_array[i].word);
		if (result != 0) {
			printf("Your function currupted %s, that is a calee saved register\n", register_name[result]);
			break;
		}
		if (received_result != test_array[i].expected_result) 
			printf("[%zd] Failed - Received: %d Expected: %d\n", i, received_result, test_array[i].expected_result);
		else 
			printf("[%zd] Passed - Received: %d Expected: %d\n", i, received_result, test_array[i].expected_result);
	}
	g_hash_table_destroy(words);
}
