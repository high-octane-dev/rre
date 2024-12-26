#include <stdio.h>
#include "parameter_block.hpp"
#include "panic.hpp"
#include "data_access.hpp"

// OFFSET: 0x00539d10
static void RemoveWhiteSpace(const char* input_str, char* output_str, int is_loaded_from_memory) {
	int input_index = 0;
	int output_index = 0;
	int bracket_depth = 0;
	int inside_quotes = 0;

	// Calculate the number of characters in the input string (excluding the null terminator)
	int input_length = strlen(input_str);

	// Initialize the output string with an empty string
	*output_str = '\0';

	// Process the input string if it contains more than one character
	if (input_length > 0) {
		do {
			switch (input_str[input_index]) {
				// Skip whitespace characters (tab, newline, carriage return, space) 
			case '\t':
			case '\n':
			case '\r':
			case ' ':
				// Only allow whitespace within quotes or inside brackets
				if ((bracket_depth > 0) || (inside_quotes != 0)) break;
				input_index++;
				continue;

				// Handle quote characters and toggle quote mode
			case '\"':
				inside_quotes = 1 - inside_quotes;
				if (is_loaded_from_memory != 0) {
					input_index++;
					continue;
				}
				break;

				// Handle opening brackets and increase bracket depth
			case '[':
				bracket_depth++;
				break;

				// Handle closing brackets and decrease bracket depth
			case ']':
				bracket_depth--;
				break;
			}

			// Copy the current character from input to output
			output_str[output_index] = input_str[input_index];
			output_index++;
			input_index++;
		} while (input_index < input_length);
	}

	// Null-terminate the output string
	output_str[output_index] = '\0';
}

// OFFSET: 0x00587e60
ParameterBlock::ParameterBlock() {
	(search).current_header_name[0] = '\0';
	(search).current_parameter_name[0] = '\0';
	(search).reset_parameter_search = 1;
	(search).current_header_index = -1;
	(search).current_parameter_index = -1;
	label_string_table = nullptr;
	headers = nullptr;
	label_string_table_len = 0;
	headers_capacity = 0;
	total_parameter_capacity = 0;
	total_parameter_list_capacity = 0;
	unk = -1;
	resource_handle = -1;
	successfully_loaded_from_memory = 0;
	unk_array_len = 0;
	unk_array_capacity = 0;
	unk_array = nullptr;
	value_string_table_len = 0;
	value_string_table = nullptr;
	(search).parent = this;

	loaded_from_memory = 0;
}

// OFFSET: 0x005c4440
ParameterBlock::~ParameterBlock() {
	FreeKeyAndHeaderMemory();
}

// OFFSET: 0x00554660
void ParameterBlock::FreeHeaders() {
	if (label_string_table != nullptr) {
		for (std::size_t i = 0; i < label_string_table_len; i++) {
			free(label_string_table[i]);
		}
		free(label_string_table);
		label_string_table = nullptr;
	}

	if (headers != nullptr) {
		for (std::size_t i = 0; i < headers_capacity; i++) {
			if (headers[i].parameters != nullptr) {
				for (std::size_t j = 0; j < headers[i].parameter_count; j++) {
					free(headers[i].parameters[j].value);
				}
				free(headers[i].parameters);
			}

			if (headers[i].parameter_lists != nullptr) {
				for (std::size_t j = 0; j < headers[i].parameter_list_count; j++) {
					for (std::size_t k = 0; k < headers[i].parameter_lists[j].token_count; k++) {
						free(headers[i].parameter_lists[j].tokens[k]);
					}
				}
				free(headers[i].parameter_lists);
			}
		}
		free(headers);
		headers = nullptr;
	}

	label_string_table_len = 0;
	headers_capacity = 0;
	total_parameter_capacity = 0;
	unk = -1;
	(search).current_header_index = -1;
	(search).current_header_name[0] = '\0';
	(search).current_parameter_index = -1;
	(search).current_parameter_name[0] = '\0';
	(search).reset_parameter_search = 1;
	(search).parent = this;
	return;
}

// OFFSET: 0x005ae1d0
void ParameterBlock::FreeKeyAndHeaderMemory() {
	FreeHeaders();
	if (resource_handle != -1) {
		int device_id = lpDataAccess->file_lookup_list[resource_handle]->device_id;
		if ((lpDataAccess->device_list[device_id].flag_data & 0x4000U) == 0 && lpDataAccess->device_list[device_id].file_pointer != nullptr) {
			fflush(lpDataAccess->device_list[device_id].file_pointer);
		}
		lpDataAccess->FClose(resource_handle);
		resource_handle = -1;
	}
	char** string_table = value_string_table;
	if (string_table != nullptr) {
		if (*string_table != nullptr) {
			free(*string_table);
		}
		free(value_string_table);
	}
	value_string_table_len = 0;
	value_string_table = nullptr;
	if (unk_array != nullptr) {
		free(unk_array);
		unk_array = nullptr;
		unk_array_len = 0;
	}
}

// OFFSET: 0x00553d60
void ParameterBlock::FreeValueStringTable() {
	char** string_table = value_string_table;
	if (string_table != nullptr) {
		if (*string_table != nullptr) {
			free(*string_table);
		}
		free(value_string_table);
	}
	value_string_table = nullptr;
	value_string_table_len = 0;
}

/*
int ParameterBlock::OpenFile(const char* name, int load_from_memory_or_not, int in_resource_handle, std::size_t* buffer, std::size_t buffer_len)
{
	int line_type;
	char* file_data = nullptr;
	char* input_line = nullptr;
	size_t string_buffer_len = 0;
	int seek_forward = 0;
	char temp[2048]{};

	FreeKeyAndHeaderMemory();
	loaded_from_memory = load_from_memory_or_not;
	label_string_table = nullptr;
	headers = nullptr;
	label_string_table_len = 0;
	headers_capacity = 0;
	total_parameter_capacity = 0;
	total_parameter_list_capacity = 0;
	unk = -1;
	resource_handle = -1;
	successfully_loaded_from_memory = 0;
	unk_array_len = 0;
	unk_array_capacity = 0;
	unk_array = nullptr;
	value_string_table_len = 0;
	value_string_table = nullptr;
	(search).parent = this;

	if (load_from_memory_or_not == 0) {
		if (in_resource_handle == -1) {
			if (buffer == nullptr) {
				resource_handle = lpDataAccess->FOpen(name, "rb");
				if (resource_handle == -1) {
					return 0;
				}
				string_buffer_len = lpDataAccess->FSize(resource_handle);
				file_data = (char*)malloc(string_buffer_len + 2);
				lpDataAccess->FRead(resource_handle, file_data, string_buffer_len, 1);
			}
			else {
				if (buffer_len == 0xFFFFFFFF) {
					buffer_len = *buffer;
					buffer = buffer + 1;
				}
				string_buffer_len = buffer_len;
				file_data = (char*)malloc(buffer_len + 2);
				memcpy(file_data, buffer, string_buffer_len);
			}
		}
		else {
			lpDataAccess->FRead(in_resource_handle, &string_buffer_len, 4);
			file_data = (char*)malloc(string_buffer_len + 2);
			lpDataAccess->FRead(in_resource_handle, file_data, string_buffer_len);
		}
		file_data[string_buffer_len] = '\r';
		file_data[string_buffer_len + 1] = '\n';
		if (headers == nullptr) {
			int header_count = 0;
			input_line = file_data;
			while (true) {
				if ((file_data + string_buffer_len <= input_line) || (*input_line == '\0')) {
					break;
				}
				line_type = CheckLine(temp, &seek_forward, input_line);
				input_line += seek_forward;
				if (line_type == 0) {
					header_count++;
				}
			}
			headers_capacity = header_count;
			if (0 < header_count) {
				headers = reinterpret_cast<Header*>(malloc(header_count * sizeof(Header)));
				for (std::size_t i = 0; i < headers_capacity; i++) {
					headers[i].flag_or_index = 0xFFFF;
					headers[i].parameters = nullptr;
					headers[i].parameter_count = 0;
					headers[i].parameter_lists = nullptr;
					headers[i].parameter_list_count = 0;
				}
			}
		}
		file_data[string_buffer_len] = '\r';
		file_data[string_buffer_len + 1] = '\n';
		input_line = file_data;
		do {
			while (true) {
				while (true) {
					while (true) {
						if ((file_data + string_buffer_len <= input_line) || (*input_line == '\0')) {
							if (search.current_header_index != -1 && search.current_header_index < headers_capacity) {
								headers_capacity = search.current_header_index + 1;
								headers = reinterpret_cast<Header*>(realloc(headers, (search.current_header_index + 1) * sizeof(Header)));
							}
							if (resource_handle != -1) {
								lpDataAccess->FClose(resource_handle);
								resource_handle = -1;
							}
							free(file_data);
							return 1;
						}
						line_type = CheckLine(temp, &seek_forward, input_line);
						input_line += seek_forward;
						if (line_type != 0) break;
						StoreNewHeader(temp, seek_forward);
					}
					if (line_type != 1) break;
					StoreNewParameter(temp, seek_forward);
				}
				if (line_type != 2) break;
				ReadCommaSeparatedParams(temp, seek_forward);
			}
		} while (line_type != -2);
		if (resource_handle != -1) {
			lpDataAccess->FClose(resource_handle);
			resource_handle = -1;
		}
		free(file_data);
		return 1;
	}
	else {
		if (load_from_memory_or_not == 1) {
			resource_handle = lpDataAccess->FOpen(name, "rb");
			if (resource_handle != -1) {
				this->successfully_loaded_from_memory = 1;
				OpenFromMemory(resource_handle);
				return 1;
			}
		}
		return 0;
	}
	return 0;
}
*/
// OFFSET: 0x005d87c0
int ParameterBlock::OpenFile(const char* name, int load_from_memory_or_not, int in_resource_handle, std::size_t* buffer, std::size_t buffer_len) {
	int line_type;
	char* file_data = nullptr;
	char* input_line = nullptr;
	size_t string_buffer_len = 0;
	int seek_forward = 0;
	char temp[2048]{};

	FreeKeyAndHeaderMemory();
	loaded_from_memory = load_from_memory_or_not;
	label_string_table = nullptr;
	headers = nullptr;
	label_string_table_len = 0;
	headers_capacity = 0;
	total_parameter_capacity = 0;
	total_parameter_list_capacity = 0;
	unk = -1;
	resource_handle = -1;
	successfully_loaded_from_memory = 0;
	unk_array_len = 0;
	unk_array_capacity = 0;
	unk_array = nullptr;
	value_string_table_len = 0;
	value_string_table = nullptr;
	(search).parent = this;

	if (load_from_memory_or_not == 0) {
		if (in_resource_handle == -1) {
			if (buffer == nullptr) {
				resource_handle = lpDataAccess->FOpen(const_cast<char*>(name), "rb");
				if (resource_handle == -1) {
					return 0;
				}
				string_buffer_len = lpDataAccess->FSize(resource_handle);
				file_data = reinterpret_cast<char*>(malloc(string_buffer_len + 2));
				lpDataAccess->FRead(resource_handle, file_data, string_buffer_len, 1);
			}
			else {
				if (buffer_len == 0xFFFFFFFF) {
					buffer_len = *buffer;
					buffer = buffer + 1;
				}
				string_buffer_len = buffer_len;
				file_data = reinterpret_cast<char*>(malloc(buffer_len + 2));
				memcpy(file_data, buffer, string_buffer_len);
			}
		}
		else {
			lpDataAccess->FRead(in_resource_handle, &string_buffer_len, 4);
			file_data = reinterpret_cast<char*>(malloc(string_buffer_len + 2));
			lpDataAccess->FRead(in_resource_handle, file_data, string_buffer_len);
		}

		file_data[string_buffer_len] = '\r';
		file_data[string_buffer_len + 1] = '\n';

		if (headers == nullptr) {
			int header_count = 0;
			input_line = file_data;

			while (input_line < file_data + string_buffer_len && *input_line != '\0') {
				line_type = CheckLine(temp, &seek_forward, input_line);
				input_line += seek_forward;
				if (line_type == 0) {
					header_count++;
				}
			}

			headers_capacity = header_count;
			if (header_count > 0) {
				headers = reinterpret_cast<Header*>(malloc(header_count * sizeof(Header)));
				for (std::size_t i = 0; i < headers_capacity; i++) {
					headers[i].flag_or_index = -1;
					headers[i].parameters = nullptr;
					headers[i].parameter_count = 0;
					headers[i].parameter_lists = nullptr;
					headers[i].parameter_list_count = 0;
				}
			}
		}

		input_line = file_data;
		while (input_line < file_data + string_buffer_len && *input_line != '\0') {
			line_type = CheckLine(temp, &seek_forward, input_line);
			input_line += seek_forward;

			if (line_type == 0) {
				StoreNewHeader(temp, seek_forward);
			}
			else if (line_type == 1) {
				StoreNewParameter(temp, seek_forward);
			}
			else if (line_type == 2) {
				ReadCommaSeparatedParams(temp, seek_forward);
			}
			else if (line_type == -2) {
				break;
			}
		}

		if (search.current_header_index != -1 && search.current_header_index < headers_capacity) {
			headers_capacity = search.current_header_index + 1;
			headers = reinterpret_cast<Header*>(realloc(headers, (search.current_header_index + 1) * sizeof(Header)));
		}

		if (resource_handle != -1) {
			lpDataAccess->FClose(resource_handle);
			resource_handle = -1;
		}

		free(file_data);
		return 1;
	}
	else if (load_from_memory_or_not == 1) {
		resource_handle = lpDataAccess->FOpen(const_cast<char*>(name), "rb");
		if (resource_handle != -1) {
			successfully_loaded_from_memory = 1;
			OpenFromMemory(resource_handle);
			return 1;
		}
	}

	return 0;
}

// OFFSET: 0x005c4450
void ParameterBlock::OpenFromMemory(int resource_handle) {
	char sanitized_header_name[1024];
	char stripped_whitespace[1024];
	char current_line[1024];

	int current_header_index = 0;
	char* success = lpDataAccess->FGets(resource_handle, current_line, 0x400);
	do {
		if (success == nullptr) {
			if (0 < this->unk_array_len) {
				this->unk_array[this->unk_array_len + -1].prev = (current_header_index - this->unk_array[this->unk_array_len + -1].prev) + -1;
			}
			return;
		}
		RemoveWhiteSpace(current_line, stripped_whitespace, this->loaded_from_memory == 0);
		if (stripped_whitespace[0] == '[') {
			int sanitized_len = strcspn(stripped_whitespace + 1, "]\n\r");
			strncpy(sanitized_header_name, stripped_whitespace + 1, sanitized_len);
			sanitized_header_name[sanitized_len] = '\0';
			if (GetHeaderIndex(sanitized_header_name) == -1) {
				if (this->unk_array == nullptr) {
					this->unk_array_capacity = 10;
					this->unk_array = (UnkArrayEntry*)malloc(10 * sizeof(UnkArrayEntry));
				}
				else if (this->unk_array_capacity <= this->unk_array_len) {
					this->unk_array_capacity = this->unk_array_capacity + 10;
					this->unk_array = (UnkArrayEntry*)realloc(this->unk_array, this->unk_array_capacity * sizeof(UnkArrayEntry));
				}
				strcpy(this->unk_array[this->unk_array_len].header_name, sanitized_header_name);
				this->unk_array[this->unk_array_len].file_offset = lpDataAccess->file_lookup_list[resource_handle]->current_offset;
				this->unk_array[this->unk_array_len].prev = current_header_index;
				if (0 < this->unk_array_len) {
					this->unk_array[this->unk_array_len + -1].prev = current_header_index - this->unk_array[this->unk_array_len + -1].prev - 1;
				}
				this->unk_array_len = this->unk_array_len + 1;
			}
		}
		current_header_index = current_header_index + 1;
		success = lpDataAccess->FGets(resource_handle, current_line, 0x400);
	} while (true);
}

// All GetParameter overloads WITH default values:

// OFFSET: 0x00588250
int ParameterBlock::GetParameter(const char* parameter, int default_value, int* dest) {
	if (search.GetParameter(parameter, dest) != 0) {
		return 1;
	}
	*dest = default_value;
	return 0;
}

// OFFSET: 0x00588280
int ParameterBlock::GetParameter(const char* parameter, unsigned int default_value, unsigned int* dest) {
	if (search.GetParameter(parameter, dest) != 0) {
		return 1;
	}
	*dest = default_value;
	return 0;
}

// OFFSET: 0x00588220
int ParameterBlock::GetParameter(const char* parameter, short default_value, short* dest) {
	if (search.GetParameter(parameter, dest) != 0) {
		return 1;
	}
	*dest = default_value;
	return 0;
}

// OFFSET: 0x005881f0
int ParameterBlock::GetParameter(const char* parameter, char default_value, char* dest) {
	if (search.GetParameter(parameter, dest) != 0) {
		return 1;
	}
	*dest = default_value;
	return 0;
}

// OFFSET: 0x005882b0
int ParameterBlock::GetParameter(const char* parameter, float default_value, float* dest) {
	if (search.GetParameter(parameter, dest) != 0) {
		return 1;
	}
	*dest = default_value;
	return 0;
}

// OFFSET: 0x005881a0
int ParameterBlock::GetParameter(const char* parameter, const char* default_value, char* dest, std::size_t dest_len) {
	if (search.GetParameter(parameter, dest) != 0) {
		return 1;
	}
	snprintf(dest, dest_len, "%s", default_value);
	return 0;
}

// OFFSET: 0x005882e0
int ParameterBlock::GetParameter(const char* parameter, Vector4* default_value, Vector4* dest) {
	return search.GetParameter(parameter, default_value, dest);
}

// All GetParameter overloads WITHOUT default values:

// OFFSET: 0x00588190
int ParameterBlock::GetParameter(const char* parameter, float* dest) {
	return search.GetParameter(parameter, dest);
}

// OFFSET: 0x00588180
int ParameterBlock::GetParameter(const char* parameter, int* dest) {
	return search.GetParameter(parameter, dest);
}

// OFFSET: 0x00588170
int ParameterBlock::GetParameter(const char* parameter, char* dest) {
	return search.GetParameter(parameter, dest);
}

// OFFSET: 0x00587fb0
int ParameterBlock::GetParameter(const char* parameter, char* dest, std::size_t dest_len) {
	if (loaded_from_memory == 0) {
		return search.GetParameter(parameter, dest, dest_len);
	}
	panic("ParameterBlock is 'loaded from memory'.");
}

// OFFSET: 0x00588300
int ParameterBlock::GetParameter(const char* parameter, int* dest, std::size_t dest_len) {
	return search.GetParameter(parameter, dest, dest_len);
}

// OFFSET: 0x005882f0
int ParameterBlock::GetParameter(const char* parameter, float* dest, std::size_t dest_len) {
	return search.GetParameter(parameter, dest, dest_len);
}

// Finally done with all the overloads...
// OFFSET: 0x00587fa0
int ParameterBlock::GetNumberOfParameterValues(const char* parameter) {
	if (search.current_header_index == -1) {
		return -1;
	}
	if (headers[search.current_header_index].parameter_count != -1) {
		int return_value = 0;
		for (std::size_t i = 0; i < headers[search.current_header_index].parameter_count; i++) {
			if (_stricmp(parameter, label_string_table[headers[search.current_header_index].parameters[i].label_string_table_index]) == 0) {
				return_value++;
			}
		}
		return return_value;
	}
	return -1;
}

// OFFSET: 0x00587ec0
int ParameterBlock::GetHeaderIndex(const char* name) {
	char buffer[1024]{};
	if (loaded_from_memory == 0) {
		strcpy(buffer, name);
		int header_index = ParseIndexedHeaderFromName(buffer);
		for (std::size_t i = 0; i < headers_capacity; i++) {
			if (header_index == headers[i].flag_or_index) {
				if (_stricmp(buffer, label_string_table[headers[i].label_string_table_index]) == 0) {
					return i;
				}
			}
		}
		return -1;
	}
	panic("ParameterBlock is 'loaded from memory'.");
}

// OFFSET: 0x00588310
void ParameterBlock::GetHeaderList(char* dest, int len) {
	char* dest_ptr = dest;
	if (loaded_from_memory == 0) {
		for (std::size_t i = 0; i < headers_capacity; i++) {
			char* name = label_string_table[headers[i].label_string_table_index];
			if (headers[i].flag_or_index != -1) {
				dest_ptr += snprintf(dest_ptr, len - (dest_ptr - dest), "%s%d", name, static_cast<int>(headers[i].flag_or_index));
				*dest_ptr = 0;
				dest_ptr++;
			}
			else {
				int name_len = strlen(name);
				std::memcpy(dest_ptr, name, name_len);
				dest_ptr += name_len;
				*dest_ptr = 0;
				dest_ptr++;
			}
		}
		return;
	}
	panic("ParameterBlock is 'loaded from memory'.");
}

// Returns 0 if Header, 1 if Parameter, 2 if comma-separated-value.
// OFFSET: 0x00554140
int ParameterBlock::CheckLine(char* output_buffer, int* length_output, char* input_buffer) const {
	// Copy characters from input buffer to output buffer.
	for (std::size_t i = 0; i < strlen(input_buffer); i++) {
		output_buffer[i] = input_buffer[i];
		// I think this is supposed to strip out the carriage returns?
		if (input_buffer[i] == '\n' && i != 0) {
			output_buffer[i - 1] = '\n';
			output_buffer[i] = 0;
			break;
		}
	}

	// Set the output length of the processed string
	*length_output = strlen(output_buffer);

	// Copy the output buffer to temporary storage
	char temp_storage[2048]{};
	strcpy(temp_storage, output_buffer);

	// Check for double quotes in the output buffer
	bool quotes_found = false;
	char* quote = strchr(output_buffer, '"');
	if ((quote != nullptr) && (quote[1] == '"')) {
		quotes_found = true;
	}

	RemoveWhiteSpace(temp_storage, output_buffer, loaded_from_memory == 0);

	// Look for '=' character in the output buffer
	char* equals = strchr(output_buffer, '=');
	char current_char = *output_buffer;

	// Determine the result code based on specific conditions
	int result_code = 0;
	if (current_char == '[') {
		result_code = 0;
	}
	else if (current_char == ';') {
		result_code = -1;
	}
	else if ((equals == nullptr) || (*equals != '=')) {
		if (current_char == 0) {
			result_code = -1;
		}
		else {
			result_code = 2;
		}
	}
	else {
		if ((quotes_found) && (equals[1] == '\0')) {
			equals[1] = '"';
			equals[2] = '"';
			equals[3] = '\0';
		}
		result_code = 1;
	}

	return result_code;
}

// OFFSET: 0x00554020
int ParameterBlock::GetOrInsertStringTableIndex(char* label) {
	if (label_string_table == nullptr) {
		label_string_table_len = 64;
		label_string_table = static_cast<char**>(malloc(64 * sizeof(char*)));
		for (int i = 0; i < label_string_table_len; ++i) {
			label_string_table[i] = nullptr;
		}
	}

	std::size_t i = 0;
	for (; i < label_string_table_len; i++) {
		if (label_string_table[i] == nullptr) {
			break;
		}
		if (_stricmp(label_string_table[i], label) == 0) {
			return i;
		}
	}

	if (i >= label_string_table_len) {
		int old_len = label_string_table_len;
		label_string_table_len += 64;
		label_string_table = static_cast<char**>(realloc(label_string_table, label_string_table_len * sizeof(char*)));
		for (int j = old_len; j < label_string_table_len; j++) {
			label_string_table[j] = nullptr;
		}
	}

	char* new_label = static_cast<char*>(malloc(strlen(label) + 1));
	std::memset(new_label, 0, strlen(label) + 1);
	label_string_table[i] = new_label;
	strcpy(new_label, label);

	return i;
}

// OFFSET: 0x005c4620
int ParameterBlock::ReadParameterBlock(const char* header) {
	if (loaded_from_memory == 0) {
		return search.ReadParameterBlock(header);
	}
	panic("ParameterBlock is 'loaded from memory'.");
}

// OFFSET: 0x00553e00
int ParameterBlock::ParseIndexedHeaderFromName(char* header_name) {
	std::size_t len = strlen(header_name);
	if (len > 1) {
		if (header_name[len - 1] >= '0' && header_name[len - 1] < ':') {
			char* digits_start = header_name + len - 1;
			while (*digits_start >= '0' && *digits_start < ':') {
				digits_start--;
			}
			int index = std::atol(digits_start);
			return index;
		}
		return -1;
	}
	return -1;
}

// OFFSET: 0x00588510
void ParameterBlock::ReadCommaSeparatedParams(char*, int) {

}

// OFFSET: 0x00553da0
void ParameterBlock::ResizeValueStringTable(int len) {
	value_string_table_len = len;
	if (len > 0) {
		value_string_table = reinterpret_cast<char**>(malloc(len * sizeof(char*)));
		*value_string_table = reinterpret_cast<char*>(malloc(value_string_table_len * 1024));
		if (value_string_table_len > 1) {
			for (std::size_t i = 1; i < value_string_table_len; i++) {
				value_string_table[i - 1] = value_string_table[i] + 1024;
			}
		}
	}
	return;
}

// OFFSET: 0x00554270
void ParameterBlock::StoreNewHeader(const char* line, int line_len) {
	char buffer[2048]{};

	if (line_len < 1 || *line != '[') {
		strcpy(buffer, line);
	}
	else {
		for (std::size_t i = 1; i < line_len; i++) {
			if (line[i] == ']' || line[i] == 0) break;
			buffer[i - 1] = line[i];
		}
	}
	short parsed_header_index = ParseIndexedHeaderFromName(buffer);

	if (search.current_header_index != -1) {
		short param_count = headers[search.current_header_index].parameter_count;
		if (param_count != 0 && param_count < total_parameter_capacity) {
			headers[search.current_header_index].parameters = reinterpret_cast<Parameter*>(realloc(headers[search.current_header_index].parameters, param_count * sizeof(Parameter)));
		}
	}
	search.current_header_index++;
	if (headers == nullptr) {
		headers_capacity = 32;
		headers = reinterpret_cast<Header*>(malloc(sizeof(Header) * headers_capacity));
		for (std::size_t i = 0; i < headers_capacity; i++) {
			headers[i].flag_or_index = -1;
			headers[i].parameters = nullptr;
			headers[i].parameter_count = 0;
			headers[i].parameter_lists = nullptr;
			headers[i].parameter_list_count = 0;
		}
	}
	else {
		int old_headers_capacity = headers_capacity;
		if (old_headers_capacity <= search.current_header_index) {
			headers_capacity = old_headers_capacity + 32;
			headers = reinterpret_cast<Header*>(realloc(headers, headers_capacity * sizeof(Header)));
			for (; old_headers_capacity < headers_capacity; old_headers_capacity++) {
				headers[old_headers_capacity].flag_or_index = -1;
				headers[old_headers_capacity].parameters = nullptr;
				headers[old_headers_capacity].parameter_count = 0;
				headers[old_headers_capacity].parameter_lists = nullptr;
				headers[old_headers_capacity].parameter_list_count = 0;
			}
		}
	}
	headers[search.current_header_index].flag_or_index = parsed_header_index;
	headers[search.current_header_index].label_string_table_index = GetOrInsertStringTableIndex(buffer);
	headers[search.current_header_index].parameter_count = 0;
	headers[search.current_header_index].parameter_list_count = 0;
	return;
}

// OFFSET: 0x00554430
void ParameterBlock::StoreNewParameter(char* line, int line_len) {
	char buffer[2048]{};

	int equal_sign = -1;
	for (std::size_t i = 0; i < line_len; i++) {
		if (line[i] == '=') {
			buffer[i] = 0;
			equal_sign = i;
			break;
		}
		buffer[i] = line[i];
	}
	if (equal_sign != -1 && line_len > (equal_sign + 1)) {
		int label_string_table_index = GetOrInsertStringTableIndex(buffer);
		std::memset(buffer, 0, 2048);
		for (std::size_t i = equal_sign + 1; i < line_len; i++) {
			if (line[i] == '\"') {
				buffer[i - equal_sign - 1] = 0;
			}
			else {
				buffer[i - equal_sign - 1] = line[i];
			}
		}
		if (headers == nullptr) {
			StoreNewHeader("None", 5);
		}
		if (headers[search.current_header_index].parameters == nullptr) {
			total_parameter_capacity = 16;
			headers[search.current_header_index].parameters = reinterpret_cast<Parameter*>(malloc(total_parameter_capacity * sizeof(Parameter)));
			for (std::size_t i = 0; i < total_parameter_capacity; i++) {
				headers[search.current_header_index].parameters[i].value = nullptr;
			}
		}
		else {
			int old_parameter_capacity = total_parameter_capacity;
			if (old_parameter_capacity <= headers[search.current_header_index].parameter_count) {
				total_parameter_capacity = old_parameter_capacity + 16;
				headers[search.current_header_index].parameters = reinterpret_cast<Parameter*>(realloc(headers[search.current_header_index].parameters, total_parameter_capacity * sizeof(Parameter)));
				for (; old_parameter_capacity < total_parameter_capacity; old_parameter_capacity++) {
					headers[search.current_header_index].parameters[old_parameter_capacity].value = nullptr;
				}
			}
		}
		if (headers[search.current_header_index].parameters[headers[search.current_header_index].parameter_count].value == nullptr) {
			headers[search.current_header_index].parameters[headers[search.current_header_index].parameter_count].value = (char*)malloc(strlen(buffer) + 1);
		}
		strcpy(headers[search.current_header_index].parameters[headers[search.current_header_index].parameter_count].value, buffer);
		headers[search.current_header_index].parameters[headers[search.current_header_index].parameter_count].label_string_table_index = label_string_table_index;
		headers[search.current_header_index].parameter_count++;
	}
	return;
}

// OFFSET: 0x00553d20
void ParameterBlock::PBSearch::SetResetParameterSearch(int reset_param_search) {
	reset_parameter_search = reset_param_search;
	current_parameter_index = -1;
}

// OFFSET: 0x005ae170
int ParameterBlock::PBSearch::ReadParameterBlock(const char* header) {
	if (parent->loaded_from_memory == 0) {
		current_header_index = parent->GetHeaderIndex(header);
		if (current_header_index == -1) {
			current_header_name[0] = '\0';
			return 0;
		}
		strncpy(current_header_name, header, 1023);
		current_header_name[1023] = 0;
		return 1;
	}
	// Unreachable!
	if (parent->loaded_from_memory != 1) {
		return 0;
	}
	return parent->ReadParameterBlock(header);
}

// OFFSET: 0x00587710
int ParameterBlock::PBSearch::GetParameter(const char* parameter, char* dest) {
	char buffer[1024]{};
	if (GetParameter(parameter, buffer, 0x400) != 0) {
		*dest = static_cast<char>(std::atol(buffer));
		return 1;
	}
	return 0;
}

// OFFSET: 0x00587980
int ParameterBlock::PBSearch::GetParameter(const char* parameter, float* dest) {
	char buffer[1024]{};
	if (GetParameter(parameter, buffer, 0x400) != 0) {
		*dest = static_cast<float>(std::atof(buffer));
		return 1;
	}
	return 0;
}

// OFFSET: 0x00587810
int ParameterBlock::PBSearch::GetParameter(const char* parameter, int* dest) {
	char buffer[1024]{};
	if (GetParameter(parameter, buffer, 0x400) == 0) {
		return 0;
	}
	int cmp = _strnicmp(buffer, "T", 1);
	if (cmp != 0) {
		cmp = _strnicmp(buffer, "ON", 2);
		if (cmp != 0) {
			cmp = _strnicmp(buffer, "Y", 1);
			if (cmp != 0) {
				cmp = _strnicmp(buffer, "F", 1);
				if (cmp != 0) {
					cmp = _strnicmp(buffer, "OFF", 3);
					if (cmp != 0) {
						cmp = _strnicmp(buffer, "N", 1);
						if (cmp != 0) {
							char* end_ptr = nullptr;
							*dest = std::strtol(buffer, &end_ptr, 0);
							return 1;
						}
					}
				}
				*dest = 0;
				return 1;
			}
		}
	}
	*dest = 1;
	return 1;
}

// OFFSET: 0x00587a00
int ParameterBlock::PBSearch::GetParameter(const char* parameter, unsigned int* dest) {
	char buffer[1024]{};
	if (GetParameter(parameter, buffer, 0x400) != 0) {
		char* end_ptr = nullptr;
		*dest = std::strtoul(buffer, &end_ptr, 0);
		return 1;
	}
	return 0;
}

// OFFSET: 0x00587790
int ParameterBlock::PBSearch::GetParameter(const char* parameter, short* dest) {
	char buffer[1024]{};
	if (GetParameter(parameter, buffer, 0x400) != 0) {
		*dest = static_cast<short>(std::atol(buffer));
		return 1;
	}
	return 0;
}

// OFFSET: 0x00587be0
int ParameterBlock::PBSearch::GetParameter(const char* parameter, float* dest, std::size_t dest_len) {
	char buffer[2048]{};
	if (GetParameter(parameter, buffer, 2048) == 0) {
		return 0;
	}
	char* start = strtok(buffer, ",");
	for (std::size_t i = 0; i < dest_len; i++) {
		if (start == nullptr) {
			return 0;
		}
		dest[i] = static_cast<float>(std::atof(start));
		start = strtok(nullptr, ",");
	}
	return 1;
}

// OFFSET: 0x00587d20
int ParameterBlock::PBSearch::GetParameter(const char* parameter, int* dest, std::size_t dest_len) {
	char buffer[2048]{};
	if (GetParameter(parameter, buffer, 2048) == 0) {
		return 0;
	}
	char* start = strtok(buffer, ",");
	for (std::size_t i = 0; i < dest_len; i++) {
		if (start == nullptr) {
			return 0;
		}
		dest[i] = std::atoi(start);
		start = strtok(nullptr, ",");
	}
	return 1;
}

// OFFSET: 0x005875e0
int ParameterBlock::PBSearch::GetParameter(const char* parameter, char* dest, std::size_t dest_len) {
	if (parent->loaded_from_memory == 0) {
		if (current_header_index != -1) {
			int parameter_count = static_cast<int>(parent->headers[current_header_index].parameter_count);
			if (parameter_count == -1) {
				return -1;
			}
			int start_index = 0;
			if (reset_parameter_search == 0 && _stricmp(parameter, current_parameter_name) == 0) {
				start_index = current_parameter_index + 1;
			}
			while (start_index < parameter_count) {
				if (_stricmp(parameter, parent->label_string_table[parent->headers[current_header_index].parameters[start_index].label_string_table_index]) == 0) {
					break;
				}
				start_index++;
			}
			if (start_index < parameter_count) {
				snprintf(dest, dest_len, "%s", parent->headers[current_header_index].parameters[start_index].value);
				current_parameter_index = start_index;
				return 1;
			}
			current_parameter_index = -1;
			return 0;
		}
	}
	else if (parent->loaded_from_memory == 1) {
		return parent->GetParameter(parameter, dest, dest_len);
	}

	return 0;
}

// OFFSET: 0x00587a90
int ParameterBlock::PBSearch::GetParameter(const char* parameter, Vector4* default_value, Vector4* dest) {
	char buffer[256]{};
	if (GetParameter(parameter, buffer, 256) == 0) {
		if (default_value != nullptr) {
			dest->x = default_value->x;
			dest->y = default_value->y;
			dest->z = default_value->z;
			dest->w = default_value->w;
		}
		return 0;
	}
	char* next = strtok(buffer, " ,");
	dest->x = static_cast<float>(std::atof(next));
	next = strtok(nullptr, " ,");
	dest->y = static_cast<float>(std::atof(next));
	next = strtok(nullptr, " ,\n");
	dest->z = static_cast<float>(std::atof(next));
	next = strtok(nullptr, " \n");
	if (next != nullptr) {
		dest->w = static_cast<float>(std::atof(next));
		return 1;
	}
	dest->w = default_value->w;
	return 1;
}
