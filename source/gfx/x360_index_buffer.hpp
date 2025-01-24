#pragma once
#include "index_buffer.hpp"

class X360IndexBuffer : public IndexBuffer {
private:
    IDirect3DIndexBuffer9* index_buffer;
    int field1_0x4;
public:
    X360IndexBuffer(D3DFORMAT fmt, int param2);
    int Create(D3DFORMAT fmt, int index_count) override;
};