#include "x360_index_buffer.hpp"

X360IndexBuffer::X360IndexBuffer(int param1, int param2) {

    this->format = D3DFMT_UNKNOWN;
    this->bytes_per_index = 0;
    this->index_count = 0;
    this->field1_0x4 = 0; 

    Create(static_cast<D3DFORMAT>(param1), param2);
}

int X360IndexBuffer::Create(D3DFORMAT fmt, int index_count) {

    return 0;
}
