#pragma once
#include "containers/container_list.hpp"
#include "game_object.hpp"

class VNObject;

struct VNObjectRef {
public:
	VNObject* unk0;
	int unk1;
	int unk2;
	int unk3;
	VNObject* unk4;
	void* data;
	ContainerList<VNObjectRef*> reference_list_1;
	ContainerList<VNObjectRef*> reference_list_2;
public:
	VNObjectRef();
	VNObjectRef(const VNObjectRef&) = delete;
	VNObjectRef& operator=(const VNObjectRef&) = delete;
	~VNObjectRef();

};

struct VNMessage {
	int unk;
	int unk1;
	int unk2;
	void* data;
};

class VirtualNetwork : public GameObject {
public:
	ContainerList<VNObjectRef*> reference_list;
public:
	VirtualNetwork(int);
	virtual ~VirtualNetwork() override;
	virtual int Tick(float dt) override;
	VNObjectRef* CreateReference(VNObject*, int);
	VNObjectRef* GetReference(VNObject*);
	int RemoveReference(VNObjectRef*);
	int SendMessage(VNObjectRef*, VNMessage*);
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