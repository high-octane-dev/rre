#include "x360_index_buffer.hpp"

// OFFSET: 0x00412cd0, STATUS: COMPLETE
X360IndexBuffer::X360IndexBuffer(D3DFORMAT fmt, int param2) : IndexBuffer() {
    field1_0x4 = 0;
    Create(fmt, param2);
}

// OFFSET: 0x00412b80, STATUS: TODO
int X360IndexBuffer::Create(D3DFORMAT fmt, int index_count) {
    return 0;
}
