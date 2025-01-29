#include "index_buffer.hpp"

// OFFSET: 0x00412870, STATUS: COMPLETE
IndexBuffer::IndexBuffer() {
	format = D3DFMT_UNKNOWN;
	bytes_per_index = 0;
	index_count = 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
IndexBuffer::~IndexBuffer() {
	format = D3DFMT_UNKNOWN;
	bytes_per_index = 0;
	index_count = 0;
}