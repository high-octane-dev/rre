#include "x360_vertex_attribute_format_string_table.hpp"
#include "x360_video_card.hpp"

// OFFSET: INLINE
X360VertexAttributeFormatStringTable::X360VertexAttributeFormatStringTable(std::size_t user_data_stride, LookupType lookup_type, std::size_t initial_string_entry_count, std::size_t string_entries_increment) : DynamicStringTable(user_data_stride, lookup_type, initial_string_entry_count, string_entries_increment) {
}

// OFFSET: 0x004125d0
X360VertexAttributeFormatStringTable::~X360VertexAttributeFormatStringTable() {
	for (std::size_t i = 0; i < len; i++) {
		if (string_entries[i].data != nullptr) {
			if (*string_entries[i].data != nullptr) {
				IDirect3DVertexDeclaration9* declaration = reinterpret_cast<IDirect3DVertexDeclaration9*>(*string_entries[i].data);
				declaration->Release();
				*string_entries[i].data = nullptr;
			}
		}
		if (string_entries[i].string != nullptr) {
			free(string_entries[i].string);
			string_entries[i].string = nullptr;
		}
		string_entries[i].requested = 0;
	}
	len = 0;
}

// OFFSET: 0x00412640
int X360VertexAttributeFormatStringTable::AddItem(char const* label, D3DVERTEXELEMENT9* elements) {
	int index = DynamicStringTable::AddItem(label, nullptr);
	if (*string_entries[index].data == nullptr) {
		IDirect3DVertexDeclaration9* declaration = nullptr;
		g_D3DDevice9->CreateVertexDeclaration(elements, &declaration);
		*string_entries[index].data = declaration;
	}
	return index;
}

std::unique_ptr<X360VertexAttributeFormatStringTable> g_lpVertexAttributeFormatStringTable = std::make_unique<X360VertexAttributeFormatStringTable>(2, StringTable::LookupType::LinearSearch, 64, 64);