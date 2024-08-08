#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFF_SIZE 4096

// Function to compare string
int strcompare(const char *str1, const char *str2){
    while(*str1 != '\0' && *str2 != '\0'){
        if(*str1 != *str2){
            return 0;
        }
        str1++;
        str2++;
    }
    return (*str1 == '\0' && *str2 == '\0');
}

// Function to find bytes size of file
long getFileSize(FILE *file) {
    long size = 0;
    char buffer[BUFF_SIZE];
    size_t bytesRead;

    // Read the file in chunks of 4096 bytes (4 Kilo Bytes)
    while((bytesRead = fread(buffer, 1, BUFF_SIZE, file)) > 0) {
        size += bytesRead;
    }

    if(ferror(file)){
        perror("Error reading file");
        return -1;
    }

    fclose(file); // Close the file
    return size;
}

size_t getLineCount(FILE *file){
    char buffer[BUFF_SIZE];
    size_t bytesRead;
    size_t lineCount = 0;

    while((bytesRead = fread(buffer, 1, BUFF_SIZE, file)) > 0){
        for(size_t i = 0; i < bytesRead; i++){
            if (buffer[i] == '\n'){
                lineCount++;
            }
        }
    }

    if(ferror(file)){
        perror("Error reading file");
        return (size_t) - 1;
    }

    if(bytesRead == 0 && buffer[BUFF_SIZE - 1] != '\n'){
        lineCount++;
    }

    fclose(file);
    return lineCount;
}

size_t getWordCount(FILE *file){
    char buffer[BUFF_SIZE];
    size_t bytesRead;
    size_t wordCount = 0;
    size_t notword = 0;

    while((bytesRead = fread(buffer, 1, BUFF_SIZE, file)) > 0){
        for(size_t i = 0; i < bytesRead; i++){
            if(isspace((unsigned char)buffer[i])){
                notword = 0;
            }else{
                if(!notword){
                    wordCount++;
                    notword = 1;
                }
            }
        }
    }

    if(ferror(file)){
        perror("Error reading file");
        return (size_t) - 1;
    }

    fclose(file);
    return wordCount;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("usage %s <filename>\n", argv[0]);
        return -1;
    }

    FILE *file = fopen(argv[2], "rb"); // Open the file in binary mode
    if (file == NULL) {
        perror("Unable to open file");
        return -1;
    }

    if(strcompare(argv[1],"-c") == 1){
        // reopen the file to reset the file pointer
        fclose(file);
        file = fopen(argv[2], "r");
        if(file == NULL){
            perror("Unable to open file");
            return -1;
        }

        long size = getFileSize(file);

        if(size != -1){
            printf("File: %s\nSize: %ld\n", argv[2], size);
        }else{
            perror("Error");
        }
    }
    
    else if (strcompare(argv[1], "-l") == 1){
        // reopen the file to reset the file pointer
        fclose(file);
        file = fopen(argv[2], "r");
        if(file == NULL){
            perror("Unable to open file");
            return -1;
        }

        size_t lineCount = getLineCount(file);
        if (lineCount == (size_t)-1) {
            return -1; // Error was handled in getLineCount
        }
        printf("File: %s\nLine count: %zu\n", argv[2], lineCount);
    }

    else if(strcompare(argv[1], "-w") == 1){
        // reopen the file to reset the file pointer
        fclose(file);
        file = fopen(argv[2], "r");
        if(file == NULL){
            perror("Unable to open file");
            return -1;
        }

        size_t wordCount = getWordCount(file);
        if(wordCount == (size_t) - 1){
            return -1;
        }
        printf("File: %s\n%zu\n", argv[2], wordCount);
    }
    fclose(file);
    return 0;
}
