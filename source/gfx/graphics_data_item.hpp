#pragma once
#include "util/macros.hpp"

class GraphicsDataItem {
protected:
	unsigned int unk;
public:
	RRE_DISABLE_COPY(GraphicsDataItem);
	inline GraphicsDataItem() : unk(0) {};
	inline virtual ~GraphicsDataItem() {}
	virtual void SendData(int) = 0;
};

#ifdef _M_IX86
static_assert(sizeof(GraphicsDataItem) == 8);
#endif