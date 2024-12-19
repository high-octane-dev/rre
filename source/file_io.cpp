#include "file_io.hpp"
#include "globals.hpp"

FILE* FOPEN(char* filename, const char* mode) {
    char resolvedPath[256]{};

    // Get the first two characters of the provided file path
    char firstChar = *filename;
    char secondChar = filename[1];

    // If the path is already an absolute or relative path starting with a '.', just open the file.
    if ((firstChar == '/') || (secondChar == ':') || (firstChar == '.' && secondChar == '\\') || (firstChar == '\\' && secondChar == '\\')) {
        return fopen(filename, mode);
    }
    // Otherwise, prepend the DataDir and then open the file.
    else {
        // If not an absolute path, prepend a base directory (g_DataDir)
        snprintf(resolvedPath, sizeof(resolvedPath), "%s%s", g_DataDir, filename);
        return fopen(resolvedPath, mode);
    }

    return nullptr;
}
