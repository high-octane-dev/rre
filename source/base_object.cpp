#include "base_object.hpp"

// OFFSET: 0x0053da70, STATUS: COMPLETE
BaseObject::BaseObject() {
	ref_count = 1;
}

// OFFSET: 0x005a4460, STATUS: COMPLETE
BaseObject::~BaseObject() {
}

// OFFSET: 0x0053da80, STATUS: COMPLETE
std::int32_t BaseObject::AddRef() {
	return ref_count++;
}

// OFFSET: 0x0053da90, STATUS: COMPLETE
std::int32_t BaseObject::RemoveRef() {
	if (ref_count != 0) {
		return ref_count--;
	}
	return 0;
}

// OFFSET: 0x0053dab0, STATUS: COMPLETE
std::int32_t BaseObject::Release() {
	ref_count--;
	if (ref_count == 0) {
		PreRelease();
		delete this;
		return 0;
	}
	return ref_count;
}

// OFFSET: 0x004013f0, STATUS: COMPLETE
std::int32_t BaseObject::GetRefCount() {
	return ref_count;
}

// OFFSET: 0x00443170, STATUS: COMPLETE
void BaseObject::PreRelease() {
}