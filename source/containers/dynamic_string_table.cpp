#include "dynamic_string_table.hpp"

// OFFSET: 0x005e47a0, STATUS: COMPLETE
DynamicStringTable::DynamicStringTable(std::size_t user_data_stride, LookupType lookup_type, std::size_t initial_string_entry_count, std::size_t string_entries_increment) : StringTable(user_data_stride, lookup_type, initial_string_entry_count, string_entries_increment, 0, 0, 4) {
    string_entries_count = initial_string_entry_count;
    string_entries = new StringEntry[initial_string_entry_count];
    user_data_heap = new void* [this->string_entries_count * this->user_data_stride];
    len = 0;
    if (lookup_type == LookupType::Hashmap) {
        GenerateFastLookupTable();
    }
}

// OFFSET: INLINE, STATUS: COMPLETE
DynamicStringTable::~DynamicStringTable() {
    DynamicStringTable::Unload();
}

// OFFSET: 0x005c97d0, STATUS: COMPLETE
int DynamicStringTable::AddItem(char const* str, void* data) {
    for (std::size_t i = 0; i < len; i++) {
        if (_stricmp(str, string_entries[i].string) == 0) {
            *string_entries[i].data = data;
            return -1;
        }
    }

    std::size_t str_len = strlen(str);
    char* dst = reinterpret_cast<char*>(malloc(str_len + 1));
    std::memcpy(dst, str, str_len);
    dst[str_len] = 0;

    int result = StringTable::AddItem(dst, data, 0);
    if (lut != nullptr) {
        lut->CHTAdd(string_entries[len - 1].string, len - 1);
    }
    return result;
}

// OFFSET: 0x00564650, STATUS: COMPLETE
void DynamicStringTable::Unload() {
    for (std::size_t i = 0; i < len; i++) {
        if (string_entries[i].string != nullptr) {
            free(string_entries[i].string);
        }
    }
    StringTable::Unload(0);
}

// OFFSET: 0x00564690, STATUS: COMPLETE
void DynamicStringTable::RemoveItem(int index) {
    if (string_entries[index].string != nullptr) {
        free(string_entries[index].string);
    }
    StringTable::RemoveItem(index);
}
