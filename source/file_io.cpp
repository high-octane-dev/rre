#include "file_io.hpp"
#include "globals.hpp"

// OFFSET: 0x0040f9e0
FILE* FOPEN(char* filename, const char* mode) {
    char resolvedPath[256]{};
    char firstChar = *filename;
    char secondChar = filename[1];

    if ((firstChar == '/') || (secondChar == ':') || (firstChar == '.' && secondChar == '\\') || (firstChar == '\\' && secondChar == '\\')) {
        return fopen(filename, mode);
    }
    else {
        snprintf(resolvedPath, sizeof(resolvedPath), "%s%s", g_DataPCDirectory, filename);
        return fopen(resolvedPath, mode);
    }

    return nullptr;
}
