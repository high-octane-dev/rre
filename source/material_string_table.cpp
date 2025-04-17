#include "material_string_table.hpp"
#include "util/rsstring_util.hpp"
#include "cars_game.hpp"

// OFFSET: 0x005c3070, STATUS: COMPLETE
MaterialStringTable::MaterialStringTable(std::size_t user_data_stride, LookupType lookup_type, std::size_t initial_string_entry_count, std::size_t string_entries_increment, std::size_t string_heap_capacity, int unk3, std::size_t max_table_markers) : StringTable(user_data_stride, lookup_type, initial_string_entry_count, string_entries_increment, string_heap_capacity, unk3, max_table_markers) {
	flags = 0;
}

// OFFSET: 0x00585fa0, STATUS: COMPLETE
int MaterialStringTable::GetIndexOrFirstDataSlot(char const* label, int _unk) {
	if (lut == nullptr) {
		for (std::size_t i = 0; i < len; i++) {
			if (_stricmp(label, string_entries[i].string) == 0) {
				return i;
			}
		}
	}
	else {
		char lower_string[260]{};
		for (std::size_t i = 0; i < sizeof(lower_string); i++) {
			if (label[i] == 0) {
				break;
			}
			lower_string[i] = std::tolower(label[i]);
		}
		int index = 0;
		if (lut->Lookup(lower_string, &index) != 0) {
			return index;
		}
	}
	if (_unk != 0) {
		for (std::size_t i = 0; i < len; i++) {
			if (string_entries[i].data != nullptr) {
				if (*string_entries[i].data != nullptr) {
					return i;
				}
			}
		}
	}
	return -1;
}

// OFFSET: 0x00586140, STATUS: COMPLETE
void MaterialStringTable::ModifyFilename(char* file_name) {
	char platform_extension[20];
	RSStringUtil::Ssnprintf(platform_extension, sizeof(platform_extension), ".%s.mst", "x360");
	strcat(file_name, platform_extension);
}

// OFFSET: 0x00550e60, STATUS: COMPLETE
int MaterialStringTable::GetVariableValue(char* dest, char* variable_name) {
	if (dest != nullptr && variable_name != nullptr) {
		if (_stricmp(variable_name, "FontTextureContentDirectory") == 0) {
			strncpy(dest, g_FontTextureContentDirectory, sizeof(g_FontTextureContentDirectory));
		}
		else {
			if (_stricmp(variable_name, "UITextureContentDirectory") == 0) {
				strncpy(dest, g_UITextureContentDirectory, sizeof(g_UITextureContentDirectory));
			}
			else {
				if (_stricmp(variable_name, "UILocalizedTextureContentDirectory") == 0) {
					strncpy(dest, g_UILocalizedTextureContentDirectory, sizeof(g_UILocalizedTextureContentDirectory));
				}
				else {
					*dest = '\0';
				}
			}
		}
		return strlen(dest);
	}
	return 0;
}
