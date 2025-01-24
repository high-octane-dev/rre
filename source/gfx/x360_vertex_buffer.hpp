#pragma once
#include "vertex_buffer.hpp"

class X360VertexBuffer : public VertexBuffer {
private:
    IDirect3DVertexBuffer9* vertex_buffer;
};
