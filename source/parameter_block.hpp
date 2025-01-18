#pragma once
#include <cstdint>
#include <cstddef>
#include "math/vector4.hpp"

struct UnkArrayEntry {
	int file_offset;
	int prev;
	char header_name[0x400];
	int unused;
	int unused2;
};

class ParameterBlock {
public:
	struct Parameter {
		int label_string_table_index;
		char* value;
	};

	struct ParameterList {
		int token_count;
		char* tokens[80];
	};

	struct Header {
		int label_string_table_index;
		// ParameterBlocks have a feature where headers that are indexed (think "ShaderPass_1" and stuff)
		// get stored differently than ones that aren't. this flag parameter is equal to that number,
		// otherwise its equal to -1.
		short flag_or_index;
		Parameter* parameters;
		short parameter_count;
		short parameter_list_count;
		// additionally, any lines that it doesn't immediately recognize as the definition of a parameter get
		// thrown in this; it splits each these lines by splitting them by commas and stores them in this array.
		// honestly, i would be very surprised if this ever isn't unused because i don't see any code that reads
		// from this /shrug
		ParameterList* parameter_lists;
	};
	class PBSearch {
	public:
		ParameterBlock* parent;
		int current_header_index;
		char current_header_name[1024];
		char current_parameter_name[1024];
		int current_parameter_index;
		int reset_parameter_search;
	public:
		void SetResetParameterSearch(int reset_param_search);
		int ReadParameterBlock(const char* header);

		int GetParameter(const char* parameter, char* dest);
		int GetParameter(const char* parameter, float* dest);
		int GetParameter(const char* parameter, int* dest);
		int GetParameter(const char* parameter, unsigned int* dest);
		int GetParameter(const char* parameter, short* dest);
		int GetParameter(const char* parameter, float* dest, std::size_t dest_len);
		int GetParameter(const char* parameter, int* dest, std::size_t dest_len);
		int GetParameter(const char* parameter, char* dest, std::size_t dest_len);

		int GetParameter(const char* parameter, const Vector4* default_value, Vector4* dest);
	};
private:
	int loaded_from_memory;
	char** label_string_table;
	Header* headers;
	int label_string_table_len;
	int headers_capacity;
	// the total capacity of all headers' parameter arrays.
	int total_parameter_capacity;
	// the total capacity of all parameters' parameter list arrays. i'm pretty sure this feature goes unused anyway /shrug
	int total_parameter_list_capacity;
	// the DataAccess resource handle.
	int resource_handle;
	// no idea what this does, at any rate the game never tries to "load a ParameterBlock from memory" so w/e
	int successfully_loaded_from_memory;
	// i have no clue what these were supposed to do, but as far as i know this goes unused anyway so idc
	int unk_array_len;
	int unk_array_capacity;
	UnkArrayEntry* unk_array;

	int value_string_table_len;
	char** value_string_table;
	// always equal to -1, no matter what. goes unused anyway so /shrug
	int unk;
public:
	class PBSearch search;
public:
	ParameterBlock();
	ParameterBlock(const ParameterBlock&) = delete;
	ParameterBlock& operator=(const ParameterBlock&) = delete;
	~ParameterBlock();

	void FreeHeaders();
	void FreeKeyAndHeaderMemory();
	void FreeValueStringTable();

	// pretty much always just set load_from_memory to 0, resource_handle to -1, buffer to nullptr, and buffer_len to 0xFFFFFFFF.
	int OpenFile(const char* name, int load_from_memory, int resource_handle, std::size_t* buffer, std::size_t buffer_len);
	void OpenFromMemory(int resource_handle);

	int GetParameter(const char* parameter, float default_value, float* dest);
	int GetParameter(const char* parameter, int default_value, int* dest);
	int GetParameter(const char* parameter, unsigned int default_value, unsigned int* dest);
	int GetParameter(const char* parameter, short default_value, short* dest);
	int GetParameter(const char* parameter, char default_value, char* dest);
	int GetParameter(const char* parameter, const char* default_value, char* dest, std::size_t dest_len);
	int GetParameter(const char* parameter, const Vector4* default_value, Vector4* dest);

	int GetParameter(const char* parameter, float* dest);
	int GetParameter(const char* parameter, int* dest);
	int GetParameter(const char* parameter, char* dest);
	int GetParameter(const char* parameter, char* dest, std::size_t dest_len);
	int GetParameter(const char* parameter, int* dest, std::size_t dest_len);
	int GetParameter(const char* parameter, float* dest, std::size_t dest_len);
	
	int GetNumberOfParameterValues(const char* parameter);
	int GetHeaderIndex(const char* name);
	void GetHeaderList(char* dest, int len);
	int CheckLine(char*,int*,char*) const;
	int GetOrInsertStringTableIndex(char*);

	int ReadParameterBlock(const char* header);
	int ParseIndexedHeaderFromName(char* name);
	void ReadCommaSeparatedParams(char*, int);
	void ResizeValueStringTable(int len);
	void StoreNewHeader(const char*, int);
	void StoreNewParameter(char*, int);
};

static_assert(sizeof(ParameterBlock::Parameter) == 8);
static_assert(sizeof(ParameterBlock::ParameterList) == 0x144);
static_assert(sizeof(ParameterBlock::Header) == 0x14);
static_assert(sizeof(ParameterBlock::PBSearch) == 0x810);
static_assert(sizeof(ParameterBlock) == 2124);