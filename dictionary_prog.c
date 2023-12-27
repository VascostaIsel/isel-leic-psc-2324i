#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <./bin/dictionary.h>

#define MAX_WORD_SIZE 32
#define SIZE_INCREMENT 64   //  Bytes, needs adjustment for structs

//  Needs size calculation to adjust increment
//  Would theoretically be 4 + 4 + 1 = 9 bytes
//  So to store 64 more structs we would need 64 * 9 = 576 bytes
typedef struct {
    int row;
    int col;
    char actualWord[MAX_WORD_SIZE];
} Word;

typedef struct {
  size_t used;
  size_t size;
  char list[][SIZE_INCREMENT];
} List;

List * addString(List * listToUpdate , const char *str){
    size_t newSize;

    //  Checks if list is full or is null, reallocates more memory
    if(!listToUpdate || listToUpdate->size == listToUpdate->used){
        newSize = listToUpdate ? listToUpdate -> size + SIZE_INCREMENT : SIZE_INCREMENT;    //  In bytes (1 char = 1 byte, adjust to struct)
        listToUpdate = realloc(listToUpdate, sizeof(*listToUpdate) + newSize * sizeof(listToUpdate -> list[0]));
        listToUpdate -> size = newSize;
        listToUpdate -> used = newSize == SIZE_INCREMENT ? 0 : listToUpdate -> used;
    }

    //  Adds to the list, increments used
    if(listToUpdate){
        strncpy(listToUpdate -> list[listToUpdate -> used], str, sizeof(listToUpdate->list[0]));
        listToUpdate -> list[listToUpdate -> used][sizeof(listToUpdate -> list[0])-1] = 0;
        listToUpdate -> used++;
    }
    return listToUpdate;
}

int main(int argc, char *argv[]){
    int isFile = 0;
    char * fileName;
    int isWord = 0;
    char * word;
    
    char * textOrWord[1];
    for(int i = 1; i < argc; ++i) {
        if(isFile && isWord){
            perror("An error occurred: Cannot have both -t and -w arguments. Please use one or the other.\n");
            return(-1);
        }else{
            if(strcmp(argv[i], "-t") == 0){
                printf("%s\n", argv[i+1]);
                fileName = argv[i+1];
                isFile = 1;
            }else if(strcmp(argv[i], "-w") == 0){
                printf("%s\n", argv[i+1]);
                word = argv[i+1];
                isWord = 1;
            }else if(strcmp(argv[i], "-d") == 0){
                printf("%s\n", argv[i+1]);
            }
        }
    }
    
    if(isFile){
        char * missingWords = checkIfExistsInDictionary(fileName, );
    }

    return 0;
}

//  f = number of files
//  w = number of words
//  O(f*w)

List * checkIfExistsInDictionary(const char * textFileToCheck, const char * dictionaryFileContributor){

    FILE * textFilePtr;
    textFilePtr = fopen(textFileToCheck, "r");

    if(textFilePtr == NULL) {
        printf("\nFailed to open file: %s\n", textFileToCheck);
        return NULL;
    }

    //  Each word limited to 32 chars
    char word[MAX_WORD_SIZE];

    //  Creates dictionary based on param file
    Dictionary * dic = dictionary_create();
    dictionary_add(dic, dictionaryFileContributor);

    int row = 0;
    int col = 0;

    List * missingWords;

    while(fscanf(textFilePtr, "%99s", word) == 1) {
        if(strcmp(word, "\n") == 0){
            row += 1;
            col = 0;
        }else{
            col += 1;
        }
    }

    fclose(textFilePtr);

    return missingWords;
}