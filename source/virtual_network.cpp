#include "virtual_network.hpp"

VirtualNetwork* lpVirtualNetwork = nullptr;

// OFFSET: 0x0056ceb0, STATUS: COMPLETE
int VNOListSort(void const* lhs, void const* rhs) {
	return ((*reinterpret_cast<VNObjectRef* const*>(lhs))->reference_list_1).Length() - ((*reinterpret_cast<VNObjectRef* const*>(rhs))->reference_list_2).Length();
}

// OFFSET: 0x005e7830, STATUS: COMPLETE
VNObject::VNObject(int param_1) {
	unk = 0;
	parent_idk = this;
	if (lpVirtualNetwork != nullptr) {
		lpVirtualNetwork->CreateReference(this, param_1);
	}
}

// OFFSET: 0x005e7860, STATUS: COMPLETE
VNObject::~VNObject() {
	if (lpVirtualNetwork != nullptr) {
		VNObjectRef* ref = lpVirtualNetwork->GetReference(this);
		if (ref != nullptr) {
			lpVirtualNetwork->RemoveReference(ref);
		}
	}
}

// OFFSET: 0x005ccf70, STATUS: TODO
VNObjectRef* VirtualNetwork::CreateReference(VNObject* object, int unk) {
	/*
	VNObjectRef* ref = GetReference(object);
	if (ref == nullptr) {
		ref = new VNObjectRef(object, unk);
		if (unk != 0) {
			AddReference(ref);
		}
		AddReferenceAndSort(ref);
	}
	*/
	return nullptr;
}

// OFFSET: 0x0059e8f0, STATUS: TODO
VNObjectRef* VirtualNetwork::GetReference(VNObject*) {
	return nullptr;
}

// OFFSET: 0x005ba8f0, STATUS: TODO
int VirtualNetwork::RemoveReference(VNObjectRef*) {
	return 0;
}

// OFFSET: 0x0059ebc0, STATUS: COMPLETE
int VirtualNetwork::SendMessage(VNObjectRef* ref, VNMessage* message) {
	for (auto& elem : ref->reference_list_2) {
		if (elem->unk0 != 0) {
			elem->unk4->ReceiveVNOEvent(elem->unk4, message);
		}
	}
	return 1; 
}

// OFFSET: 0x0059e7c0, STATUS: COMPLETE
VNObjectRef::VNObjectRef() : reference_list_1(), reference_list_2() {
	unk0 = nullptr;
	unk1 = 0;
	unk2 = 0;
	unk3 = 0;
	unk4 = nullptr;
	data = nullptr;
	reference_list_1.CLNonMacroCreate(1, 1, std::numeric_limits<std::int32_t>::max());
	reference_list_2.CLNonMacroCreate(64, 128, std::numeric_limits<std::int32_t>::max());
}

// OFFSET: INLINE, STATUS: TODO
VNObjectRef::~VNObjectRef() {
}
