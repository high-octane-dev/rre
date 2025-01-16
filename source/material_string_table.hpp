#pragma once
#include "containers/string_table.hpp"

class MaterialStringTable : public StringTable
{
public:
	MaterialStringTable(std::size_t user_data_stride, std::size_t generate_lut, std::size_t initial_string_entry_count, std::size_t string_entries_increment, std::size_t string_heap_capacity, int unk3, std::size_t max_table_markers);
	virtual void ModifyFilename(char*) override;
	virtual int GetVariableValue(char*, char*) override;
};