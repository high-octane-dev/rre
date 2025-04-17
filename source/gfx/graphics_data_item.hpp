#pragma once

class GraphicsDataItem {
protected:
	unsigned int unk;
public:
	inline GraphicsDataItem() : unk(0) {};
	GraphicsDataItem(const GraphicsDataItem&) = delete;
	GraphicsDataItem& operator=(const GraphicsDataItem&) = delete;
	inline virtual ~GraphicsDataItem() {}
	virtual void SendData(int) = 0;
};

#ifdef _M_IX86
static_assert(sizeof(GraphicsDataItem) == 8);
#endif