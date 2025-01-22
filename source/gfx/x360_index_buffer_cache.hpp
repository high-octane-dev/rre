#pragma once
#include "cache.hpp"
#include "x360_index_buffer.hpp"

class X360IndexBufferCache {
private:
	X360IndexBuffer* index_buffer;
	StaticCache* cache;
};

