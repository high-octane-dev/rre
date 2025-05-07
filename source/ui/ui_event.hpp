#pragma once

class UIFlashAnimationListener {
public:
	virtual void AnimationDone() = 0;
};

class UIEvent {
public:
	inline virtual ~UIEvent() {}
};

class UIInputEvent : public UIEvent {
public:
	int device_id;
	int unk1;
	int key_count;
	int key_values[100];
	char key_states[100];
	int unk2;
	int unk3;
public:
	inline UIInputEvent() {
		device_id = -1;
		unk2 = 0;
		key_count = 0;
	}
};

class UIControllerEvent;

class UIInputEventListener {
public:
	inline virtual ~UIInputEventListener() {}
	virtual void ProcessKeydown(UIInputEvent*) = 0;
	virtual void ProcessKeyup(UIInputEvent*) = 0;
};

class UIControllerEventListener {
	inline virtual ~UIControllerEventListener() {}
	virtual void ControllerDetached(UIControllerEvent*) = 0;
	virtual void ControllerAttached(UIControllerEvent*) = 0;
};

#ifdef _M_IX86
static_assert(sizeof(UIFlashAnimationListener) == 4);
static_assert(sizeof(UIEvent) == 4);
static_assert(sizeof(UIInputEvent) == 0x20c);
static_assert(sizeof(UIInputEventListener) == 4);
#endif