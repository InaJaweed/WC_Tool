#include <stdio.h>
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
long getFileSize(const char *filename) {
    long size = 0;
    // Open filename
    FILE *file = fopen(filename, "rb");
    if(file == NULL){
        perror("Unable to open file");
        return -1;
    }

    char buffer[BUFF_SIZE];
    size_t bytesRead;

    // Read the file in chunks of 4096 bytes (4 Kilo Bytes)
    while((bytesRead = fread(buffer, 1, BUFF_SIZE, file)) > 0) {
        size += bytesRead;
    }

    fclose(file); // Close the file
    return size;
}

size_t getLineCount(const char *filename){
    FILE *file = fopen(filename, "r"); // Open file in read mode
    if(file == NULL){
        perror("Unable to open file");
        return -1;
    }

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

    if(bytesRead == 0 && buffer[bytesRead - 1] != '\n'){
        lineCount++;
    }

    fclose(file);
    return lineCount;
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
        long size = getFileSize(argv[2]);

        if(size != -1){
            printf("File: %s\nSize = %ld\n",argv[2], size);
        }else{
            perror("Error");
        }
    }
    
    else if (strcompare(argv[1], "-l") == 1)
    {
        size_t lineCount = getLineCount(argv[2]);
        if (lineCount == (size_t)-1) {
            return -1; // Error was handled in getLineCount
    }

        printf("File: %s\nLine count: %zu\n",argv[2], lineCount);
    }
    return 0;
}
