#pragma once
#include "containers/container_list.hpp"
#include "game_object.hpp"

class VNObject;
struct VNObjectRef;

class VirtualNetwork : public GameObject {
public:
	ContainerList<VNObjectRef*> refs;
public:
	VNObjectRef* CreateReference(VNObject*, int);
	VNObjectRef* GetReference(VNObject*);
	int RemoveReference(VNObjectRef*);
};

struct VNMessage {
	int unk;
	int unk1;
	int unk2;
	void* data;
};

class VNObject {
protected:
	int unk;
	VNObject* parent_idk;
public:
	VNObject(int);
	virtual ~VNObject();
	virtual void ReceiveVNOData(VNObject*, void*) = 0;
	virtual void ReceiveVNOEvent(VNObject*, VNMessage*) = 0;
};

extern VirtualNetwork* lpVirtualNetwork;