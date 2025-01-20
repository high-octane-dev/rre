#include <memory>

#include "game_object.hpp"

static int g_SearchesOpen = 0;
static int g_TotalGameObjects = 0;

constexpr std::uint32_t kWantsNothing = 0x0;
constexpr std::uint32_t kWantsTickWhileInactive = 0x1;
constexpr std::uint32_t kWantsTick = 0x2;
constexpr std::uint32_t kWantsRenderPre3D = 0x4;
constexpr std::uint32_t kWantsRender3D = 0x8;
constexpr std::uint32_t kWantsRenderPost3D = 0x10;
constexpr std::uint32_t kWantsPause = 0x20;
constexpr std::uint32_t kWantsLostFocus = 0x40;
constexpr std::uint32_t kWantsRestore = 0x80;
constexpr std::uint32_t kWantsEventHandler = 0x100;
constexpr std::uint32_t kWantsKeyUpHandler = 0x200;
constexpr std::uint32_t kWantsKeyDownHandler = 0x400;
constexpr std::uint32_t kWantsPacketHandler = 0x800;
constexpr std::uint32_t kWantsPrepareGeometry = 0x1000;
constexpr std::uint32_t kWantsAllEvents = 0xffffffff;

// OFFSET: 0x0054c6f0, STATUS: COMPLETE
std::int32_t GameObject::Release() {
	if (g_SearchesOpen == 0) {

		if (GetRefCount() < 2) {
			// Is this accurate?
			flags.enabled = true;
			RemoveFromChain();
		}

		if (firstChild != nullptr) {
			firstChild->ReleaseSiblingsInReverseCreationOrder();
		}
		ref_count--;

		if (ref_count != 0) {
			return ref_count;
		}
		PreRelease();
		delete this;
	}
	return 0;
}

// OFFSET: 0x0054bf30, STATUS: COMPLETE
void GameObject::Disable() {
	flags.enabled = false;
	flags.packetEnabled = false;
	AncestorsDisabled();
}

// OFFSET: 0x0054bf50, STATUS: COMPLETE
void GameObject::DisableAll() {
	flags.enabled = false;
	flags.packetEnabled = false;
	GameObject* child = firstChild;
	while (child != nullptr) {
		if (!child->flags.killMe) {
			child->DisableAll();
		}
		child = child->next;
	}
}

// OFFSET: 0x0054bf40, STATUS: COMPLETE
void GameObject::Enable() {
	flags.enabled = true;
	flags.packetEnabled = true;
	AncestorsEnabled();
}

// OFFSET: 0x0054bf80, STATUS: COMPLETE
void GameObject::EnableAll() {
	flags.enabled = true;
	flags.packetEnabled = true;
	GameObject* child = firstChild;
	while (child != nullptr) {
		if (!child->flags.killMe) {
			child->EnableAll();
		}
		child = child->next;
	}
}

// OFFSET: 0x0054bef0, STATUS: COMPLETE
void GameObject::AncestorsDisabled() {
	GameObject* child = firstChild;
	while (child != nullptr) {
		if (!child->flags.killMe) {
			child->AncestorsDisabled();
		}
		child = child->next;
	}
}

// OFFSET: 0x0054bf10, STATUS: COMPLETE
void GameObject::AncestorsEnabled() {
	GameObject* child = firstChild;
	while (child != nullptr) {
		if (!child->flags.killMe) {
			child->AncestorsEnabled();
		}
		child = child->next;
	}
}

// OFFSET: 0x0054c1c0, STATUS: COMPLETE
void GameObject::EnableEvents(std::uint32_t mask) {
	descendantsWantedEventMask |= mask;
	if (parent != nullptr) {
		parent->descendantsWantedEventMask = parent->descendantsWantedEventMask | descendantsWantedEventMask | wantedEventMask;
		UpPropogateMasks();
	}
	return;
}

// OFFSET: 0x0054c1f0, STATUS: COMPLETE
void GameObject::DisableEvents(std::uint32_t mask) {
	wantedEventMask &= ~mask;
}

// OFFSET: 0x0055aad0, STATUS: COMPLETE
GameObject* GameObject::Create() {
	return this;
}

// OFFSET: 0x0054c400, STATUS: COMPLETE
int GameObject::TickWhileInactive(float deltaSeconds) {
	if (descendantsWantedEventMask & kWantsTickWhileInactive) {
		GameObject* child = firstChild;
		while (child != nullptr) {
			if (child->flags.isPaused && !child->flags.isPaused && !child->flags.killMe) {
				child->TickWhileInactive(deltaSeconds);
			}
			child = child->next;
		}
	}
	return 1;
}

// OFFSET: 0x0054c390, STATUS: COMPLETE
int GameObject::Tick(float deltaSeconds) {
	if ((descendantsWantedEventMask & kWantsTick) != 0) {
		for (GameObject* child = firstChild; child != nullptr; child = child->next) {
			if (child->flags.enabled && !child->flags.isPaused && !child->flags.killMe) {
				if ((child->wantedEventMask & kWantsTick) == 0) {
					if ((child->descendantsWantedEventMask & kWantsTick) != 0) {
						child->Tick(deltaSeconds);
					}
				}
				else {
					child->Tick(deltaSeconds);
				}
			}
		}
		return 1;
	}
	return 1;
}

// OFFSET: 0x0054f370, STATUS: COMPLETE
int GameObject::SimulationTick(float deltaSeconds) {
	return 1;
}

// OFFSET: 0x0054c230, STATUS: COMPLETE
int GameObject::PrepareGeometry() {
	if ((descendantsWantedEventMask & kWantsPrepareGeometry) == 0) {
		return 1;
	}
	for (GameObject* child = firstChild; child != nullptr; child = child->next) {
		if (child->flags.enabled && !child->flags.killMe) {
			if ((child->wantedEventMask & kWantsPrepareGeometry) == 0) {
				if ((child->descendantsWantedEventMask & kWantsPrepareGeometry) != 0) {
					child->PrepareGeometry();
				}
			}
			else {
				child->PrepareGeometry();
			}
		}
	}
	return 1;
}

// OFFSET: 0x0054c280, STATUS: COMPLETE
int GameObject::RenderPre3D() {
	if ((descendantsWantedEventMask & kWantsRenderPre3D) == 0) {
		return 1;
	}
	for (GameObject* child = firstChild; child != nullptr; child = child->next) {
		if (child->flags.enabled && !child->flags.killMe) {
			if ((child->wantedEventMask & kWantsRenderPre3D) == 0) {
				if ((child->descendantsWantedEventMask & kWantsRenderPre3D) != 0) {
					child->RenderPre3D();
				}
			}
			else {
				child->RenderPre3D();
			}
		}
	}
	return 1;
}

// OFFSET: 0x0054c2d0, STATUS: COMPLETE
int GameObject::Render3D(int userData) {
	if ((descendantsWantedEventMask & kWantsRender3D) == 0) {
		return 1;
	}
	GameObject* child = firstChild;
	if (child == nullptr) {
		return 1;
	}
	do {
		if (child->flags.enabled && !child->flags.killMe) {
			if ((child->wantedEventMask & kWantsRender3D) == 0) {
				if ((child->descendantsWantedEventMask & kWantsRender3D) != 0) {
					child->Render3D(userData);
				}
			}
			else {
				child->Render3D(userData);
			}
		}
		child = child->next;
	} while (child != nullptr);
	return 1;
}

// OFFSET: 0x0054c340, STATUS: COMPLETE
int GameObject::RenderPost3D() {
	if ((descendantsWantedEventMask & kWantsRenderPost3D) == 0) {
		return 1;
	}
	for (GameObject* child = firstChild; child != nullptr; child = child->next) {
		if (child->flags.enabled && !child->flags.killMe) {
			if ((child->wantedEventMask & kWantsRenderPost3D) == 0) {
				if ((child->descendantsWantedEventMask & kWantsRenderPost3D) != 0) {
					child->RenderPost3D();
				}
			}
			else {
				child->RenderPost3D();
			}
		}
	}
	return 1;
}

// OFFSET: 0x0054c450, STATUS: COMPLETE
int GameObject::Pause(int bPaused) {
	flags.isPaused = bPaused;
	if ((descendantsWantedEventMask & kWantsPause) == 0) {
		return 1;
	}
	for (GameObject* child = firstChild; child != nullptr; child = child->next) {
		if (!child->flags.killMe && ((child->wantedEventMask & kWantsPause) != 0)) {
			child->Pause(bPaused);
		}
	}
	return 1;
}

// OFFSET: 0x0054c4f0, STATUS: COMPLETE
int GameObject::LostFocus() {
	if ((descendantsWantedEventMask & kWantsLostFocus) != 0) {
		for (GameObject* child = firstChild; child != nullptr; child = child->next) {
			if (!child->flags.killMe) {
				child->LostFocus();
			}
		}
		return 1;
	}
	return 1;
}

// OFFSET: 0x0054c4b0, STATUS: COMPLETE
int GameObject::Restore() {
	if ((descendantsWantedEventMask & kWantsRestore) != 0) {
		for (GameObject* child = firstChild; child != nullptr; child = child->next) {
			if (!child->flags.killMe) {
				child->Restore();
			}
		}
		return 1;
	}
	return 1;
}

// OFFSET: 0x0054c530, STATUS: COMPLETE
int GameObject::EventHandler(std::int32_t eventNumber) {
	if ((descendantsWantedEventMask & kWantsEventHandler) != 0) {
		for (GameObject* child = firstChild; child != nullptr; child = child->next) {
			if (child->flags.enabled && !child->flags.killMe && child->EventHandler(eventNumber)) {
				return 1;
			}
		}
	}
	return 0;
}

// OFFSET: 0x0054c580, STATUS: COMPLETE
int GameObject::KeyUpHandler(ProcessNode* event, KeyInfo* keyInfo) {
	if ((descendantsWantedEventMask & kWantsKeyUpHandler) != 0) {
		for (GameObject* child = firstChild; child != nullptr; child = child->next) {
			if ((child->flags.enabled && !child->flags.killMe) && child->KeyUpHandler(event, keyInfo)) {
				return 1;
			}
		}
	}
	return 0;
}

// OFFSET: 0x0054c5e0, STATUS: COMPLETE
int GameObject::KeyDownHandler(ProcessNode* event, KeyInfo* keyInfo) {
	if ((descendantsWantedEventMask & kWantsKeyDownHandler) != 0) {
		for (GameObject* child = firstChild; child != nullptr; child = child->next) {
			if ((child->flags.enabled && !child->flags.killMe) && child->KeyDownHandler(event, keyInfo)) {
				return 1;
			}
		}
	}
	return 0;
}

// OFFSET: 0x0054c640, STATUS: COMPLETE
int GameObject::PacketHandler(std::uint32_t message, void* lpMsg, std::uint32_t idFrom, std::uint32_t idTo, std::uint32_t receiveTime) {
	if ((descendantsWantedEventMask & kWantsPacketHandler) != 0) {
		for (GameObject* child = firstChild; child != nullptr; child = child->next) {
			if ((child->flags.packetEnabled && !child->flags.killMe) && child->PacketHandler(message, lpMsg, idFrom, idTo, receiveTime)) {
				return 1;
			}
		}
	}
	return 0;
}

// OFFSET: 0x0054c200, STATUS: COMPLETE
void GameObject::Kill() {
	for (GameObject* object = firstChild; object != nullptr; object = object->next) {
		object->Kill();
	}
	flags.killMe = true;
}

// OFFSET: 0x00583e70, STATUS: TODO
void GameObject::DumpHierarchy(char*, DumpType, std::int32_t) {
}

// OFFSET: 0x0054c000, STATUS: COMPLETE
GameObject* GameObject::AddChild(GameObject* child, unsigned int eventMask) {
	if (g_SearchesOpen != 0) {
		return 0;
	}
	
	if (child != nullptr) {
		GameObject* lastChild = this->firstChild;
		if (lastChild == nullptr) {
			this->firstChild = child;
			child->prev = nullptr;
		}
		else {
			while (lastChild->next != nullptr) {
				lastChild = lastChild->next;
			}
			lastChild->next = child;
			child->prev = lastChild;
		}

		GameObject* current = child;
		while (current != nullptr) {
			current->parent = this;
			current = current->next;
		}

		child->wantedEventMask = eventMask;
		if (child->parent != nullptr) {
			GameObject* parent = child->parent;
			parent->descendantsWantedEventMask |= child->descendantsWantedEventMask | eventMask;
			parent->UpPropogateMasks();
		}
	}

	return child;
}

// OFFSET: 0x005837d0, STATUS: COMPLETE
void GameObject::DumpHierarchy(FILE*, DumpType, std::int32_t) {
}

// OFFSET: 0x0054bea0, STATUS: COMPLETE
GameObject::GameObject(bool startEnabled) : BaseObject() {
	(g_TotalGameObjects)++;
	prev = nullptr;
	next = nullptr;
	firstChild = nullptr;
	parent = nullptr;
	wantedEventMask = 0;
	descendantsWantedEventMask = 0;
	flags.enabled = startEnabled;
	flags.packetEnabled = startEnabled;
	flags.isPaused = false;
	flags.killMe = false;
	ref_count = 1;
	typeID = 0xFF;
}

// OFFSET: 0x0054bfb0, STATUS: COMPLETE
void GameObject::UpPropogateMasks() {
	GameObject* object = this;
	while (object->parent != nullptr) {
		object->parent->descendantsWantedEventMask = object->parent->descendantsWantedEventMask | object->descendantsWantedEventMask | object->wantedEventMask;
		object = parent;
	}
	return;
}

// OFFSET: 0x0054c070, STATUS: COMPLETE
int GameObject::RemoveFromChain() {
	if (g_SearchesOpen != 0) {
		return 0;
	}

	if (prev != nullptr) {
		prev->next = next;
	}
	if (next != nullptr) {
		next->prev = prev;
	}

	if (parent != nullptr && parent->firstChild == this) {
		parent->firstChild = next;
	}

	GameObject* root = parent;
	while (root != nullptr && root->parent != nullptr) {
		root = root->parent;
	}
	if (root != this && !root->flags.enabled && root->descendantsWantedEventMask != 0) {
		root->ReEvaluateDescendantsWantedEventMasks();
	}

	parent = nullptr;
	next = nullptr;
	prev = nullptr;

	return 1;
}

// OFFSET: 0x0054c6b0, STATUS: COMPLETE
int GameObject::ReleaseSiblingsInReverseCreationOrder() {
	if (g_SearchesOpen != 0) {
		return 0;
	}

	if (next != nullptr) {
		next->ReleaseSiblingsInReverseCreationOrder();
	}

	if (prev != nullptr) {
		prev->next = nullptr;
	}

	next = nullptr;

	return Release();
}

// OFFSET: 0x0054bfd0, STATUS: COMPLETE
void GameObject::ReEvaluateDescendantsWantedEventMasks() {
	GameObject* child = firstChild;
	descendantsWantedEventMask = 0;

	while (child != nullptr) {
		child->ReEvaluateDescendantsWantedEventMasks();
		descendantsWantedEventMask |= child->descendantsWantedEventMask | child->wantedEventMask;
		child = child->next;
	}
}

// OFFSET: 0x005a4480, STATUS: COMPLETE
GameObject::~GameObject() {
	(g_TotalGameObjects)--;
}