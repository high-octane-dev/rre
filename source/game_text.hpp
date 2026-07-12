#pragma once
#include "containers/container_hash_table.hpp"

struct AppensionInfo {
    unsigned char appension_count;
    int* appends; // Array of string indexes to add.
    short* at_positions; // Array of positions, for where each string should be added.
    char* copied_to_end; // Copied at the end of the string.
};

class StringInfo {
public:
    unsigned short max_size;
    char* value;
    int flags;
    bool is_dynamic;
    AppensionInfo* appension_info;
    unsigned char is_appended_to_len; // Number of strings this string is added to.
    int* is_appended_to; // Array of string indices this string is added to.

    inline StringInfo() {
        this->max_size = 0;
        this->value = nullptr;
        this->flags = 0;
        this->is_dynamic = false;
        this->appension_info = nullptr;
        this->is_appended_to_len = 0;
        this->is_appended_to = 0;
    }

    inline ~StringInfo() {
        if (this->appension_info != nullptr) {
            if (this->appension_info->appends != nullptr) {
                delete[] this->appension_info->appends;
            }
            if (this->appension_info->at_positions != nullptr) {
                delete[] this->appension_info->at_positions;
            }
            delete this->appension_info;
        }
        if (this->is_appended_to != nullptr) {
            delete[] this->is_appended_to;
        }
        return;
    }
};

class GameText {
private:
    char name[16];
    int string_count;
    char** text_ids;
    ContainerHashTable<char*, int>* text_id_to_value_index;
    StringInfo* string_infos;
    int unused;
    char* text_id_buffer;
    char* value_buffer;
public:
	const char* GetString(const char* label);
};