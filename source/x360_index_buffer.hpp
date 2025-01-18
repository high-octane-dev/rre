#ifndef X360_INDEX_BUFFER_HPP
#define X360_INDEX_BUFFER_HPP
#include "gfx/vertex_buffer.hpp"

class X360IndexBuffer : public X360VertexBuffer {
public:
    X360IndexBuffer(int param_1, int param_2);

    void Initialize(int param_1, int param_2);

private:
    static void* vftable;
};

#endif

