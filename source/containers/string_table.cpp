#include "string_table.hpp"
#include "util/token_string.hpp"
#include <data_access.hpp>

// OFFSET: 0x00563e90
StringTable::StringTable(std::size_t user_data_stride, LookupType lookup_type, std::size_t initial_string_entry_count, std::size_t string_entries_increment, std::size_t string_heap_capacity, int unk3, std::size_t max_table_markers) {
    this->unk = 1;
    this->user_data_stride = user_data_stride;
    this->string_entries_increment = string_entries_increment;
    this->lookup_type = lookup_type;
    this->initial_string_entry_count = initial_string_entry_count;
    this->len = 0;
    this->string_entries_count = 0;
    this->string_heap = nullptr;
    this->user_data_heap = nullptr;
    this->string_heap_end = 0;
    this->unk2 = 0;
    this->string_entries = nullptr;
    this->unused_base_entries = nullptr;
    this->unused_base_entries_len = 0;
    this->lut = nullptr;
    this->string_heap_capacity = string_heap_capacity;
    this->unk3 = unk3;
    this->max_table_markers = max_table_markers;
    if (max_table_markers < 1) {
        this->table_markers = nullptr;
    }
    else {
        this->table_markers = new int[max_table_markers];
    }
    this->table_marker_index = 0;
}

// OFFSET: 0x005b6910
StringTable::~StringTable() {
    if (this->unused_base_entries != nullptr) {
        for (std::size_t i = 0; i < this->unused_base_entries_len; i++) {
            free(this->unused_base_entries[i].string);
            free(this->unused_base_entries[i].data);
        }
        delete[] this->unused_base_entries;
    }

    if (this->string_entries != nullptr) {
        delete[] this->string_entries;
    }
    if (this->string_heap != nullptr) {
        free(this->string_heap);
    }
    if (this->user_data_heap != nullptr) {
        delete[] this->user_data_heap;
    }
    if (this->table_markers != nullptr) {
        delete[] this->table_markers;
    }
}

int StringTable::Load(char* path) {
    this->len = 0;

    if (LoadStringsBufferFromFile(path) == 0) {
        return 0;
    }

    std::size_t string_count = this->len;
    this->string_entries_count = this->len;

    if (this->len > 0) {
        if (string_count < this->initial_string_entry_count) {
            string_count = this->initial_string_entry_count;
        }
        this->string_entries_count = string_count;
        this->string_entries = new StringEntry[string_count];

        this->user_data_heap = new void*[this->string_entries_count * this->user_data_stride];

        int entry_index = 0;
        bool in_string = false;
        for (int i = 0; i < this->string_heap_end; i++) {
            if (this->string_heap[i] == '\0') {
                in_string = false;
            }
            else if (!in_string) {
                this->string_entries[entry_index].string = &this->string_heap[i];
                this->string_entries[entry_index].data = &this->user_data_heap[entry_index * this->user_data_stride];
                memset(this->string_entries[entry_index].data, 0, this->user_data_stride * sizeof(void*));
                this->string_entries[entry_index].requested = 0;
                in_string = true;
                entry_index++;
            }
        }
    }

    if (this->unused_base_entries_len > 0) {
        for (int base_index = 0; base_index < this->unused_base_entries_len; base_index++) {
            for (int entry_index = 0; entry_index < this->len; entry_index++) {
                if (strcmp(this->unused_base_entries[base_index].string, this->string_entries[entry_index].string) == 0) {
                    this->string_entries[entry_index].string = this->unused_base_entries[base_index].string;
                    this->string_entries[entry_index].data = this->unused_base_entries[base_index].data;
                    this->string_entries[entry_index].requested = this->unused_base_entries[base_index].requested;
                    break;
                }
            }
        }
    }

    if (this->lookup_type == LookupType::Hashmap) {
        GenerateFastLookupTable();
    }

    return this->len;
}

// OFFSET: 0x00563f60
void StringTable::Unload(int release)
{
    if (this->string_entries != nullptr) {
        int base_entry_index = 0;
        for (std::size_t base_entry_index = 0; base_entry_index < this->unused_base_entries_len; base_entry_index++) {
            for (std::size_t string_index = 0; string_index < len; string_index++) {
                if (_stricmp(this->string_entries[string_index].string, this->unused_base_entries[base_entry_index].string) == 0) {
                    this->unused_base_entries[base_entry_index].string = this->string_entries[string_index].string;
                    this->unused_base_entries[base_entry_index].data = this->string_entries[string_index].data;
                    this->unused_base_entries[base_entry_index].requested = this->string_entries[string_index].requested;
                    break;
                }
            }
        }

        this->len = 0;
        if (release != 0) {
            delete[] this->string_entries;
            this->string_entries = nullptr;
        }
    }
    
    if (release != 0) {
        delete[] this->user_data_heap;
        this->user_data_heap = nullptr;
    }

    if (this->string_heap != nullptr) {
        free(this->string_heap);
        this->string_heap = nullptr;
        this->string_heap_end = 0;
    }

    this->table_marker_index = 0;
    this->len = 0;

    return;
}

// OFFSET: 0x00564500
int StringTable::Request(int index) {
    string_entries[index].requested++;
	return string_entries[index].requested;
}

// OFFSET: 0x005860e0
void StringTable::Request(int index, void* user_data)
{
    this->string_entries[index].data[1] = user_data;
}

// OFFSET: 0x005642d0
int StringTable::AddItem(char* string, void* user_data, int use_string_heap) {
    if (this->string_entries == nullptr) {
        this->string_entries_count = this->initial_string_entry_count;
        this->string_entries = new StringEntry[this->initial_string_entry_count];
        this->len = 0;
        this->user_data_heap = new void*[this->user_data_stride * this->string_entries_count];
    }
    else {
        if (this->len >= this->string_entries_count) {
            this->string_entries_count += this->string_entries_increment;
            StringEntry* new_table = new StringEntry[this->string_entries_count];
            memcpy(new_table, this->string_entries, this->len * sizeof(StringEntry));
            delete[] this->string_entries;
            this->string_entries = new_table;

            void** new_user_data_heap = new void* [this->string_entries_count * this->user_data_stride];
            memcpy(new_user_data_heap, this->user_data_heap, this->user_data_stride * this->len * sizeof(void*));

            for (std::size_t i = 0; i < this->len; i++) {
                this->string_entries[i].data = &new_user_data_heap[i * this->user_data_stride];
            }

            delete[] this->user_data_heap;
            this->user_data_heap = new_user_data_heap;
        }
    }

    char* lowercase_string;
    if (!use_string_heap) {
        lowercase_string = _strlwr(string);
        this->string_entries[this->len].string = lowercase_string;
    }
    else {
        char* end_ptr = string + 1;
        while (*end_ptr != '\0') {
            ++end_ptr;
        }

        std::size_t required_heap_size = (end_ptr - string) + 1 + this->string_heap_end;
        if (this->unk2 < required_heap_size) {
            std::size_t new_heap_size = this->unk2 + this->unk3;
            this->unk2 = new_heap_size;

            char* new_heap = reinterpret_cast<char*>(realloc(this->string_heap, new_heap_size));
            char* heap_end_ptr = new_heap + this->string_heap_end;

            while (heap_end_ptr < new_heap + this->unk2) {
                *heap_end_ptr++ = '\0';
            }

            this->string_heap = new_heap;
        }

        char* heap_start = this->string_heap + this->string_heap_end;
        while (*string != '\0') {
            *heap_start = *string;
            ++heap_start;
            ++string;
        }
        *heap_start = '\0';
        lowercase_string = _strlwr(this->string_heap + this->string_heap_end);

        this->string_entries[this->len].string = lowercase_string;
        this->string_heap_end += (end_ptr - string) + 1;
    }

    // Assign user data and clear memory
    this->string_entries[this->len].data = &this->user_data_heap[this->len * this->user_data_stride];
    memset(this->string_entries[this->len].data, 0, this->user_data_stride * sizeof(int));
    *this->string_entries[this->len].data = user_data;

    this->string_entries[this->len].requested = 0;
    this->len += 1;

    return this->len - 1;
}

// OFFSET: 0x00564540
void StringTable::Report()
{
}

// OFFSET: 0x00564550
void StringTable::RemoveItem(int index)
{
    this->string_entries[index].string = nullptr;
    for (std::size_t i = 0; i < user_data_stride; i++) {
        this->string_entries[index].data[i] = nullptr;
    }
    this->string_entries[index].string = nullptr;
    std::size_t new_len = this->len + -1;
    if (index == new_len) {
        this->len = new_len;
    }
    return;
}

// OFFSET: 0x005645b0
void StringTable::PlaceTableMarker() {
    this->table_markers[this->table_marker_index] = this->len;
    this->table_marker_index++;
}

// OFFSET: 0x005645d0
void StringTable::RemoveTableMarker() {
    table_marker_index--;
}

// OFFSET: 0x005645e0
void StringTable::RemoveAllMarkers()
{
    while (this->table_marker_index != 0) {
        RemoveTableMarker();
    }
}

// OFFSET: 0x00564610
void StringTable::RemoveAllAfterMarker(int index) {
    for (std::size_t i = this->table_markers[index]; i < this->len; i++) {
        RemoveItem(i);
    }
    this->len = this->table_markers[index];
}

void StringTable::ModifyFilename(char*)
{
}

int StringTable::GetVariableValue(char*, char*)
{
    return 0;
}

// OFFSET: 0x00595420
void StringTable::DestroyFastLookupTable() {
    if (this->lut != nullptr) {
        delete this->lut;
        this->lut = nullptr;
    }
}

// OFFSET: 0x005e4700
void StringTable::GenerateFastLookupTable() {
    DestroyFastLookupTable();
    this->lut = new ContainerHashTable<char*, int>();
    this->lut->CHTCreateFull(2039, 500, StringHashValueFunction, StringHashCompareFunction);
    for (std::size_t i = 0; i < this->len; i++) {
        this->lut->CHTAdd(this->string_entries[i].string, i);
    }
}

// OFFSET: 0x00595360
int StringTable::GetIndex(char* string) {
    int found_index = -1;

    char lower_string[260]{};
    for (std::size_t i = 0; i < sizeof(lower_string); i++) {
        if (string[i] == 0) {
            break;
        }
        lower_string[i] = std::tolower(string[i]);

    }

    if (this->lut == nullptr) {
        if (this->string_entries == nullptr) {
            return -1;
        }

        for (int i = 0; i < this->len; ++i) {
            if (strcmp(this->string_entries[i].string, lower_string) == 0) {
                found_index = i;
                break;
            }
        }
    }
    else {
        int lut_index = -1;
        if (this->lut->Lookup(lower_string, &lut_index) != 0) {
            found_index = lut_index;
        }
    }
    
    return found_index;
}

// OFFSET: 0x005e43f0
int StringTable::LoadStringsBufferFromFile(char* path) {
    char normalized_filename[260]{};
    char lowercase_filename[260]{};
    char line_buffer[260]{};
    size_t bytes_read = 0;
    int file_handle = -1;
    char* leftover_data = nullptr;
    std::size_t leftover_len = 0;

    snprintf(normalized_filename, sizeof(normalized_filename), "%s", path);
    this->ModifyFilename(normalized_filename);

    for (std::size_t i = 0; i < sizeof(lowercase_filename); i++) {
        if (normalized_filename[i] == 0) {
            break;
        }
        lowercase_filename[i] = std::tolower(normalized_filename[i]);

    }

    file_handle = lpDataAccess->FOpen(lowercase_filename, "rb");
    if (file_handle == -1) {
        return 0;
    }

    if (!this->string_heap) {
        this->string_heap = static_cast<char*>(malloc(this->string_heap_capacity));
        memset(this->string_heap, 0, this->string_heap_capacity);
        this->unk2 = this->string_heap_capacity;
    }

    char* read_data = static_cast<char*>(malloc(5000));
    if (!read_data) {
        lpDataAccess->FClose(file_handle);
        return 0;
    }

    while (true) {
        memset(read_data, 0, 5000);


        if (leftover_len != 0) {
            strncpy(read_data, line_buffer, leftover_len);
        }

        bytes_read = lpDataAccess->FRead(file_handle, read_data + leftover_len, 5000 - leftover_len);
        leftover_data = nullptr;
        leftover_len = 0;

        if (bytes_read < 1) {
            break;
        }

        _strlwr(read_data);
        read_data[5000 - 1] = '\0';
        read_data[5000 - 2] = -1;

        char* result = ParseThisBlock(read_data);
        if (result != nullptr) {
            leftover_data = result;
            leftover_len = strlen(result);
            strncpy(line_buffer, result, sizeof(line_buffer));
        }
        else {
            leftover_data = nullptr;
            leftover_len = 0;
        }
    }

    lpDataAccess->FClose(file_handle);
    free(read_data);
    return 1;
}

// OFFSET: 0x005e45b0
char* StringTable::ParseThisBlock(char* input_block) {
    char* current_token;
    char* token_end;
    int token_length;
    char include_path[260] = { 0 };

    TokenString token_parser{ input_block, 1, 0 };

    while (current_token = token_parser.NextToken("\r\n\0"), current_token != nullptr) {
        token_end = current_token + strlen(current_token);

        token_length = token_end - current_token;
        for (std::size_t i = 0; i < token_length; i++) {
            if (current_token[i] == -1 && token_length > 1) {
                return current_token;
            }
        }

        if (*current_token == ';') {
            continue;
        }

        if (*current_token == '#') {
            char* include_directive = strstr(current_token, "#include");
            if (include_directive) {
                char* path_start = strstr(current_token, "\"");
                if (path_start) {
                    path_start++;
                    char* path_end = strchr(path_start, '\"');
                    if (path_end) {
                        std::size_t path_length = path_end - path_start;
                        strncpy(include_path, path_start, path_length);
                        include_path[path_length] = '\0';
                        LoadStringsBufferFromFile(include_path);
                    }
                }
            }
        }
        else {
            StoreThisLine(current_token);
        }
    }

    return nullptr;
}

void StringTable::ResolveString(char* string) {
    char* start_var = string;
    char* end_var = nullptr;
    char resolved_value[260] = { 0 };
    char variable_name[260] = { 0 };
    std::size_t offset = 0;

    while ((start_var = strchr(string + offset, '$')) != nullptr) {
        if (start_var[1] == '(') {
            end_var = strchr(start_var, ')');
            if (end_var != nullptr) {
                size_t var_length = end_var - (start_var + 2);
                strncpy(variable_name, start_var + 2, var_length);
                variable_name[var_length] = '\0';
                size_t resolved_length = this->GetVariableValue(resolved_value, variable_name);
                if (resolved_length > 0) {
                    size_t remaining_length = strlen(end_var + 1);
                    memmove(start_var + resolved_length, end_var + 1, remaining_length + 1);
                    memcpy(start_var, resolved_value, resolved_length);
                }
                offset += (end_var - string) + 1;
            }
        }
    }
}

// OFFSET: 0x00564170
int StringTable::StoreThisLine(char* line) {
    char current_char;
    bool has_dollar = false;
    char processed_line[260] = { 0 };
    int processed_length = 0;
    int input_length = strlen(line);

    if (input_length <= 0) {
        return 0;
    }

    for (int i = 0; i < input_length; ++i) {
        current_char = line[i];
        if (current_char == -1) {
            if (input_length < 2) {
                return 0;
            }
        }
        else if (current_char != ' ') {
            processed_line[processed_length++] = current_char;
            if (current_char == '$') {
                has_dollar = true;
            }
        }
    }

    if (has_dollar) {
        ResolveString(processed_line);
    }

    int total_length = processed_length + 1;
    if (this->string_heap_end + total_length >= this->unk2) {
        size_t new_size = this->unk2 + this->unk3;
        this->unk2 = new_size;
        this->string_heap = reinterpret_cast<char*>(realloc(this->string_heap, new_size));
        memset(this->string_heap + this->string_heap_end, 0, new_size - this->string_heap_end);
    }

    memcpy(this->string_heap + this->string_heap_end, processed_line, processed_length);
    this->string_heap_end += processed_length;

    if (processed_line[processed_length - 1] != '\0') {
        this->string_heap[this->string_heap_end++] = '\0';
    }

    this->len++;
    return 1;
}
