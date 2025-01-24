#pragma once
#include "string_table.hpp"

// In DynamicStringTable, individual strings are allocated seperately, rather than just being pointers into the `string_heap`.
class DynamicStringTable : public StringTable {
public:
	DynamicStringTable(std::size_t user_data_stride, LookupType lookup_type, std::size_t initial_string_entry_count, std::size_t string_entries_increment);
	int AddItem(char const*, void*);
	// NOT virtual btw.
	void Unload();

	virtual ~DynamicStringTable() override;
	virtual void RemoveItem(int) override;
};