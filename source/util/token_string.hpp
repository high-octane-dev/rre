#pragma once
#include <cstring>

struct TokenString {
	int skip_first_delim;
	int unk2;
	char* string;
	char* next_string;

	inline TokenString(char* mod_string, int should_skip_first_delim, int _unk2) {
        skip_first_delim = should_skip_first_delim;
        unk2 = _unk2;
        string = nullptr;
        if (strlen(mod_string)) {
            next_string = mod_string;
        }
        else {
            next_string = nullptr;
        }
	}

	inline char* NextToken(const char* delimiters) {

        if (!this->next_string || *this->next_string == '\0') {
            this->next_string = nullptr;
            return 0;
        }

        char* current_pos = this->next_string;

        if (this->skip_first_delim != 0) {
            while (*current_pos && strchr(delimiters, *current_pos)) {
                ++current_pos;
            }
            if (*current_pos == '\0') {
                this->next_string = nullptr;
                return nullptr;
            }
        }

        this->string = current_pos;

        if (this->unk2 == 0) {
            current_pos = strpbrk(current_pos, delimiters);
        }
        else {
            while ((current_pos = strpbrk(current_pos, delimiters)) != nullptr) {
                if ((current_pos[1] & 0x80U) == 0) {
                    break;
                }
                ++current_pos;
            }
        }

        if (current_pos) {
            *current_pos = '\0';
            this->next_string = current_pos + 1;
            if (*this->next_string == '\0') {
                this->next_string = nullptr;
            }
        }
        else {
            this->next_string = nullptr;
        }

        return this->string;
	}
};