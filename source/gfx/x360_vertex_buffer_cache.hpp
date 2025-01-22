#pragma once
#include "cache.hpp"
#include "x360_vertex_buffer.hpp"

class X360VertexBufferCache {
private:
	X360VertexBuffer* vertex_buffer;
	StaticCache* cache;
};
