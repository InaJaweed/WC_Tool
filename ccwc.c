#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFF_SIZE 4096

// Function to compare two strings for equality
int strcompare(const char *str1, const char *str2) {
    while(*str1 != '\0' && *str2 != '\0') {
        if(*str1 != *str2) {
            return 0; // Strings are not equal
        }
        str1++;
        str2++;
    }
    // Return 1 if both strings ended simultaneously, indicating equality
    return (*str1 == '\0' && *str2 == '\0');
}

// Function to calculate the size of a file
long int getFileSize(FILE *file) {
    long size = 0;
    char buffer[BUFF_SIZE];
    size_t bytesRead;

    // Read the file in chunks of 4096 bytes
    while((bytesRead = fread(buffer, 1, BUFF_SIZE, file)) > 0) {
        size += bytesRead; // Accumulate the total bytes read
    }

    if(ferror(file)) {
        perror("Error reading file"); // Print error if reading fails
        return -1;
    }

    fclose(file); // Close the file after reading
    return size;
}

// Function to count the number of lines in a file
size_t getLineCount(FILE *file) {
    char buffer[BUFF_SIZE];
    size_t bytesRead;
    size_t lineCount = 0;

    // Read the file in chunks of 4096 bytes
    while((bytesRead = fread(buffer, 1, BUFF_SIZE, file)) > 0) {
        for(size_t i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n') {
                lineCount++; // Increment line count for each newline character
            }
        }
    }

    if(ferror(file)) {
        perror("Error reading file"); // Print error if reading fails
        return (size_t) -1;
    }

    // If the last chunk does not end with a newline, count the last line
    if(bytesRead == 0 && buffer[BUFF_SIZE - 1] != '\n') {
        lineCount++;
    }

    fclose(file); // Close the file after reading
    return lineCount;
}

// Function to count the number of words in a file
size_t getWordCount(FILE *file) {
    char buffer[BUFF_SIZE];
    size_t bytesRead;
    size_t wordCount = 0;
    size_t notword = 0; // Flag to indicate if the last character was not a word character

    // Read the file in chunks of 4096 bytes
    while((bytesRead = fread(buffer, 1, BUFF_SIZE, file)) > 0) {
        for(size_t i = 0; i < bytesRead; i++) {
            if(isspace((unsigned char)buffer[i])) {
                notword = 0; // Reset the flag when a whitespace is found
            } else {
                if(!notword) {
                    wordCount++; // Increment word count when a new word is encountered
                    notword = 1; // Set the flag to indicate that we are in a word
                }
            }
        }
    }

    if(ferror(file)) {
        perror("Error reading file"); // Print error if reading fails
        return (size_t) -1;
    }

    fclose(file); // Close the file after reading
    return wordCount;
}

int main(int argc, char *argv[]) {
    // Check if sufficient arguments are provided
    if(argc < 3) {
        printf("usage %s <option> <filename>\n", argv[0]);
        return -1;
    }

    // Open the specified file in read mode
    FILE *file = fopen(argv[2], "r");
    if (file == NULL) {
        perror("Unable to open file"); // Print error if file cannot be opened
        return -1;
    }

    // Process the command-line option
    if(strcompare(argv[1], "-c") == 1) {
        // Reopen the file to reset the file pointer
        fclose(file);
        file = fopen(argv[2], "r");
        if(file == NULL) {
            perror("Unable to open file");
            return -1;
        }

        long size = getFileSize(file); // Get the size of the file

        if(size != -1) {
            printf("File: %s\nSize: %ld\n", argv[2], size);
        } else {
            perror("Error");
        }
    }
    // Line count option
    else if (strcompare(argv[1], "-l") == 1) {
        // Reopen the file to reset the file pointer
        fclose(file);
        file = fopen(argv[2], "r");
        if(file == NULL) {
            perror("Unable to open file");
            return -1;
        }

        size_t lineCount = getLineCount(file); // Get the number of lines

        if (lineCount == (size_t)-1) {
            return -1; // Error was handled in getLineCount
        }
        printf("File: %s\nLine count: %zu\n", argv[2], lineCount);
    }
    // Word count option
    else if(strcompare(argv[1], "-w") == 1) {
        // Reopen the file to reset the file pointer
        fclose(file);
        file = fopen(argv[2], "r");
        if(file == NULL) {
            perror("Unable to open file");
            return -1;
        }

        size_t wordCount = getWordCount(file); // Get the number of words

        if(wordCount == (size_t) -1) {
            return -1;
        }
        printf("File: %s\n%zu\n", argv[2], wordCount);
    }
    
    fclose(file); // Close the file before exiting
    return 0;
}
