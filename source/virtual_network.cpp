#include "virtual_network.hpp"

VirtualNetwork* lpVirtualNetwork = nullptr;

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
VNObjectRef* VirtualNetwork::CreateReference(VNObject*, int) {
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
