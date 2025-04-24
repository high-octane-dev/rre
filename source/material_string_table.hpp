#pragma once
#include "containers/string_table.hpp"

// Maps strings to X360SurfaceMaterial*s.
class MaterialStringTable : public StringTable {
private:
	std::uint8_t flags;
public:
	MaterialStringTable(std::size_t user_data_stride, LookupType lookup_type, std::size_t initial_string_entry_count, std::size_t string_entries_increment, std::size_t string_heap_capacity, int unk3, std::size_t max_table_markers);

	int GetIndexOrFirstDataSlot(char const*, int);
	virtual void ModifyFilename(char*) override;
	virtual int GetVariableValue(char*, char*) override;
};