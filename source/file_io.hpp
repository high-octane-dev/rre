#pragma once
#include <cstdio>

FILE* FOPEN(char* filename, const char* mode);
int FCLOSE(FILE* stream);
size_t FREAD(void* buffer, size_t size, size_t count, FILE* stream);
unsigned int FREAD(void* buffer, unsigned int numberOfBytes, FILE* stream);
unsigned int FREAD_ASYNC(void* buffer, unsigned int numberOfBytes, unsigned int maxBufferSize, FILE* stream);
void FREAD_ASYNC_ABORT(FILE* stream);
int FREAD_ASYNC_STATUS(FILE* stream, unsigned int* bytesRead, int* diskError);
size_t FWRITE(void* buffer, size_t size, size_t count, FILE* stream);
int FPRINTF(FILE* stream, const char* format ...);
int FSEEK(FILE* stream, long offset, int origin);
int FSEEK(unsigned int sector);
long FTELL(FILE* stream);
char* FGETS(char* s, int size, FILE* stream);
int FGETC(FILE* stream);
int SET_MODE(FILE* stream, int mode);
int FEOF(FILE* stream);
int FERROR(FILE* stream);
int FFLUSH(FILE* stream);
void SPLIT_PATH(const char* path, char* drive, char* dir, char* fname, char* ext);
