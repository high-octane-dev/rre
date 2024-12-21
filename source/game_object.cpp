#include <memory>

#include "game_object.hpp"

static int SearchesOpen = 0;
static int TotalGameObjects = 0;

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

// OFFSET: 0x0054c6f0
std::int32_t GameObject::Release()
{
	if (SearchesOpen == 0) {

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

// OFFSET: 0x0054bf30
void GameObject::Disable()
{
	flags.enabled = false;
	flags.packetEnabled = false;
	AncestorsDisabled();
}

// OFFSET: 0x0054bf50
void GameObject::DisableAll()
{
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

// OFFSET: 0x0054bf40
void GameObject::Enable()
{
	flags.enabled = true;
	flags.packetEnabled = true;
	AncestorsEnabled();
}

// OFFSET: 0x0054bf80
void GameObject::EnableAll()
{
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

// OFFSET: 0x0054bef0
void GameObject::AncestorsDisabled()
{
	GameObject* child = firstChild;
	while (child != nullptr) {
		if (!child->flags.killMe) {
			child->AncestorsDisabled();
		}
		child = child->next;
	}
}

// OFFSET: 0x0054bf10
void GameObject::AncestorsEnabled()
{
	GameObject* child = firstChild;
	while (child != nullptr) {
		if (!child->flags.killMe) {
			child->AncestorsEnabled();
		}
		child = child->next;
	}
}

// OFFSET: 0x0054c1c0
void GameObject::EnableEvents(std::uint32_t param_1)
{
	descendantsWantedEventMask |= param_1;
	if (parent != nullptr) {
		parent->descendantsWantedEventMask = parent->descendantsWantedEventMask | descendantsWantedEventMask | wantedEventMask;
		UpPropogateMasks();
	}
	return;
}

// OFFSET: 0x0054c1f0
void GameObject::DisableEvents(std::uint32_t mask)
{
	wantedEventMask &= ~mask;
}

// OFFSET: 0x0055aad0
GameObject* GameObject::Create()
{
	return this;
}

// OFFSET: 0x0054c400
int GameObject::TickWhileInactive(float deltaSeconds)
{
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

// OFFSET: 0x0054c390
int GameObject::Tick(float deltaSeconds)
{
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

// OFFSET: 0x0054f370
int GameObject::SimulationTick(float deltaSeconds)
{
	return 1;
}

// OFFSET: 0x0054c230
int GameObject::PrepareGeometry()
{
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

// OFFSET: 0x0054c280
int GameObject::RenderPre3D()
{
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

// OFFSET: 0x0054c2d0
int GameObject::Render3D(int userData)
{
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

// OFFSET: 0x0054c340
int GameObject::RenderPost3D()
{
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

// OFFSET: 0x0054c450
int GameObject::Pause(int bPaused)
{
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

// OFFSET: 0x0054c4f0
int GameObject::LostFocus()
{
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

// OFFSET: 0x0054c4b0
int GameObject::Restore()
{
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

// OFFSET: 0x0054c530
int GameObject::EventHandler(std::int32_t eventNumber)
{
	if ((descendantsWantedEventMask & kWantsEventHandler) != 0) {
		for (GameObject* child = firstChild; child != nullptr; child = child->next) {
			if (child->flags.enabled && !child->flags.killMe && child->EventHandler(eventNumber)) {
				return 1;
			}
		}
	}
	return 0;
}

// OFFSET: 0x0054c580
int GameObject::KeyUpHandler(ProcessNode* event, KeyInfo* keyInfo)
{
	if ((descendantsWantedEventMask & kWantsKeyUpHandler) != 0) {
		for (GameObject* child = firstChild; child != nullptr; child = child->next) {
			if ((child->flags.enabled && !child->flags.killMe) && child->KeyUpHandler(event, keyInfo)) {
				return 1;
			}
		}
	}
	return 0;
}

// OFFSET: 0x0054c5e0
int GameObject::KeyDownHandler(ProcessNode* event, KeyInfo* keyInfo)
{
	if ((descendantsWantedEventMask & kWantsKeyDownHandler) != 0) {
		for (GameObject* child = firstChild; child != nullptr; child = child->next) {
			if ((child->flags.enabled && !child->flags.killMe) && child->KeyDownHandler(event, keyInfo)) {
				return 1;
			}
		}
	}
	return 0;
}

// OFFSET: 0x0054c640
int GameObject::PacketHandler(std::uint32_t message, void* lpMsg, std::uint32_t idFrom, std::uint32_t idTo, std::uint32_t receiveTime)
{

	if ((descendantsWantedEventMask & kWantsPacketHandler) != 0) {
		for (GameObject* child = firstChild; child != nullptr; child = child->next) {
			if ((child->flags.packetEnabled && !child->flags.killMe) && child->PacketHandler(message, lpMsg, idFrom, idTo, receiveTime)) {
				return 1;
			}
		}
	}
	return 0;
}

// OFFSET: 0x0054c200
void GameObject::Kill()
{
	for (GameObject* object = firstChild; object != nullptr; object = object->next) {
		object->Kill();
	}
	flags.killMe = true;
}

// OFFSET: 0x00583e70
void GameObject::DumpHierarchy(char*, DumpType, std::int32_t)
{
}

// OFFSET: 0x0054c000
GameObject* GameObject::AddChild(GameObject* child, unsigned int eventMask) {
	if (SearchesOpen != 0) {
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

// OFFSET: 0x005837d0
void GameObject::DumpHierarchy(FILE*, DumpType, std::int32_t)
{
}

// OFFSET: 0x0054bea0
GameObject::GameObject(bool startEnabled) : BaseObject()
{
	(TotalGameObjects)++;
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

// OFFSET: 0x0054bfb0
void GameObject::UpPropogateMasks()
{
	GameObject* object = this;
	while (object->parent != nullptr) {
		object->parent->descendantsWantedEventMask = object->parent->descendantsWantedEventMask | object->descendantsWantedEventMask | object->wantedEventMask;
		object = parent;
	}
	return;
}

// OFFSET: 0x0054c070
int GameObject::RemoveFromChain()
{
	if (SearchesOpen != 0) {
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

// OFFSET: 0x0054c6b0
int GameObject::ReleaseSiblingsInReverseCreationOrder()
{
	if (SearchesOpen != 0) {
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

// OFFSET: 0x0054bfd0
void GameObject::ReEvaluateDescendantsWantedEventMasks()
{
	GameObject* child = firstChild;
	descendantsWantedEventMask = 0;

	while (child != nullptr) {
		child->ReEvaluateDescendantsWantedEventMasks();
		descendantsWantedEventMask |= child->descendantsWantedEventMask | child->wantedEventMask;
		child = child->next;
	}
}

// OFFSET: 0x005a4480
GameObject::~GameObject()
{
	(TotalGameObjects)--;
}