#include "file_io.hpp"
#include "globals.hpp"

// OFFSET: 0x0040f9e0, STATUS: COMPLETE
FILE* FOPEN(char* filename, const char* mode) {
    char resolved_path[256]{};
    char first_char = *filename;
    char second_char = filename[1];

    if ((first_char == '/') || (second_char == ':') || (first_char == '.' && second_char == '\\') || (first_char == '\\' && second_char == '\\')) {
        return fopen(filename, mode);
    }
    else {
        snprintf(resolved_path, sizeof(resolved_path), "%s%s", g_DataPCDirectory, filename);
        return fopen(resolved_path, mode);
    }

    return nullptr;
}
