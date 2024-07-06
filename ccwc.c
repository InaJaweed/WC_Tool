#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Function to find bytes size of file
long getFileSize(const char *filename) {
    long size = 0;
    // Open filename
    FILE *file = fopen(filename, "rb");
    if(file == NULL){
        perror("Unable to open file");
        return -1;
    }

    char buffer[4096];
    size_t bytesRead;

    // Read the file in chunks of 4096 bytes (4 Kilo Bytes)
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        size += bytesRead;
    }

    fclose(file); // Close the file
    return size;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("usage %s <filename>\n", argv[0]);
        return -1;
    }

    FILE *file = fopen(argv[1], "rb"); // Open the file in binary mode
    if (file == NULL) {
        perror("Unable to open file");
        return -1;
    }
 
    long size = getFileSize(argv[1]);

    if(size != -1){
        printf("Size = %ld\n", size);
    }else{
        printf("Error");
    }

    return 0;
}
