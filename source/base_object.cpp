#include "base_object.hpp"

BaseObject::BaseObject()
{
	ref_count = 1;
}

BaseObject::~BaseObject()
{
}

std::int32_t BaseObject::AddRef()
{
	return ref_count++;
}

std::int32_t BaseObject::RemoveRef()
{
	if (ref_count != 0) {
		return ref_count--;
	}
	return 0;
}

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

std::int32_t BaseObject::GetRefCount()
{
	return ref_count;
}

void BaseObject::PreRelease()
{
}