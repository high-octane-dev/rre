#pragma once
#include "serializable_interface.hpp"

class Cars2BumperSticker : public SerializableInterface {
public:
	int index;
	int material_index;
	int unlocked;
public:
	// OFFSET: 0x0042af60, STATUS: COMPLETE
	inline virtual int Serialize(void* buffer, int len) override {
		*reinterpret_cast<int*>(buffer) = unlocked;
		return sizeof(int);
	}
	// OFFSET: 0x0042af80, STATUS: COMPLETE
	inline virtual int DeSerialize(void* buffer, int len) override {
		unlocked = *reinterpret_cast<int*>(buffer);
		return sizeof(int);
	}
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual int GetSerializedDataSize() override {
		return 0;
	}
};

#ifdef _M_IX86
static_assert(sizeof(Cars2BumperSticker) == 16);
#endif