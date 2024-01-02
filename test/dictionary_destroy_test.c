#include <stdio.h>
#include "dictionary.h"

const char *register_name[] = { "", "rbp", "rbx", "r15", "r14", "r13", "r12" };

int invoke_and_test(void *, void (*)(), int, ...);

void dictionary_destroy(Dictionary *dictionary);

int main() {        
    Dictionary *test_dictionary = dictionary_create();
    int received_result; // not used
    int result = invoke_and_test(&received_result, (void (*)())dictionary_destroy, 1, test_dictionary);
    if (result != 0) {
		printf("Your function currupted %s, that is a calee saved register\n", register_name[result]);
		return -1;
	}

	printf("[0] Passed\n");
	
	/*
	* Since there isnt a way to find if it was actually destroyed, we are assuming
	* that the function did its purpose.
	*
	* The only way to see if it fails is by checking on the console if there is a
	* segmentation fault print.
	*/
	
	// dictionary_lookup(test_dictionary, "test); would cause a segmentation fault
}
