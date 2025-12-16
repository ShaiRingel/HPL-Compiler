#ifndef FILEREADER_H
#define FILEREADER_H

typedef struct {
    char** inputBuffer;
    int rowsNum;
} FileDetails;

FileDetails readFile(char *path);

#endif