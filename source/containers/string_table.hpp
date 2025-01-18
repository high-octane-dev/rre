#pragma once
#include "base_object.hpp"
#include "container_hash_table.hpp"

class StringTable : public BaseObject {
public:
	struct StringEntry {
		char* string;
		void** data;
		int requested;
	};
	enum class LookupType {
		LinearSearch,
		Hashmap,
	};
protected:
	int unk;
	std::size_t len;
	std::size_t string_entries_count;
	std::size_t user_data_stride;
	char* string_heap;
	void** user_data_heap;
	int unk2;
	std::size_t string_heap_end;
	std::size_t initial_string_entry_count;
	std::size_t string_entries_increment;
	std::size_t string_heap_capacity;
	int unk3;
	StringEntry* string_entries;
	ContainerHashTable<char*, int>* lut;
	LookupType lookup_type;
	StringEntry* unused_base_entries;
	std::size_t unused_base_entries_len;
	int* table_markers;
	std::size_t max_table_markers;
	std::size_t table_marker_index;
public:
	StringTable(std::size_t user_data_stride, LookupType lookup_type, std::size_t initial_string_entry_count, std::size_t string_entries_increment, std::size_t string_heap_capacity, int unk3, std::size_t max_table_markers);
	StringTable(const StringTable&) = delete;
	StringTable& operator=(const StringTable&) = delete;
	virtual ~StringTable() override;
	virtual int Load(char*);
	virtual void Unload(int);
	virtual int Request(int);
	virtual void Request(int, void*);
	virtual int AddItem(char* string, void* user_data, int use_string_heap);
	virtual void Report();
	virtual void RemoveItem(int);
	virtual void PlaceTableMarker();
	virtual void RemoveTableMarker();
	virtual void RemoveAllMarkers();
	virtual void RemoveAllAfterMarker(int);
	virtual void ModifyFilename(char*);
	virtual int GetVariableValue(char*, char*);

	void DestroyFastLookupTable();
	void GenerateFastLookupTable();
	int GetIndex(char* string);
	int LoadStringsBufferFromFile(char* path);
	char* ParseThisBlock(char* block);
	void ResolveString(char* string);
	int StoreThisLine(char* line);
};