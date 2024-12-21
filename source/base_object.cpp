#include "base_object.hpp"

// OFFSET: 0x0053da70
BaseObject::BaseObject()
{
	ref_count = 1;
}

// OFFSET: 0x005a4460
BaseObject::~BaseObject()
{
}

// OFFSET: 0x0053da80
std::int32_t BaseObject::AddRef()
{
	return ref_count++;
}

// OFFSET: 0x0053da90
std::int32_t BaseObject::RemoveRef()
{
	if (ref_count != 0) {
		return ref_count--;
	}
	return 0;
}

// OFFSET: 0x0053dab0
std::int32_t BaseObject::Release()
{
	ref_count--;
	if (ref_count == 0) {
		PreRelease();
		delete this;
		return 0;
	}
	return ref_count;
}

// OFFSET: 0x004013f0
std::int32_t BaseObject::GetRefCount()
{
	return ref_count;
}

// OFFSET: 0x00443170
void BaseObject::PreRelease()
{
}