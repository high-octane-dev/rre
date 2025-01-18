#include "x360_index_buffer.hpp"
#include <gfx/vertex_buffer.hpp>

// Placeholder for vtable initialization
void* X360IndexBuffer::vftable = nullptr;

// OFFSET: 0x00412cd0
X360IndexBuffer::X360IndexBuffer(int param_1, int param_2)
    : X360VertexBuffer(param_1, param_2) { // Corrected line
    IndexBuffer::format = D3DFMT_UNKNOWN;
    IndexBuffer::bytes_per_index = 0;
    IndexBuffer::index_count = 0;
    IndexBuffer::unk = 0;

    IndexBuffer::vt = vftable;

    Initialize(param_1, param_2);
}
