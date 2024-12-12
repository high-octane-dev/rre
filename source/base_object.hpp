#pragma once
#include <cstdint>

class BaseObject {
public:
	BaseObject();
	virtual ~BaseObject();
	virtual std::int32_t AddRef();
	virtual std::int32_t RemoveRef();
	virtual std::int32_t Release();
	virtual std::int32_t GetRefCount();
	virtual void PreRelease();
protected:
	std::int32_t refCount;
};