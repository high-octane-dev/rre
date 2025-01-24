#pragma once
#include <d3d9.h>
#include <memory>
#include "containers/dynamic_string_table.hpp"

class X360VertexAttributeFormatStringTable : public DynamicStringTable {
public:
	X360VertexAttributeFormatStringTable(std::size_t user_data_stride, LookupType lookup_type, std::size_t initial_string_entry_count, std::size_t string_entries_increment);
	virtual ~X360VertexAttributeFormatStringTable() override;
	int AddItem(char const*, D3DVERTEXELEMENT9*);
};

// Yet again, looks like the original game leaks this global *facepalm*. So we'll use std::unique_ptr.
extern std::unique_ptr<X360VertexAttributeFormatStringTable> g_lpVertexAttributeFormatStringTable;