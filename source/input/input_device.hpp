#pragma once
#include "util/macros.hpp"

struct Button {
	char reserved[0x10];
};

enum class SubType : int {
	Invalid = -1,
	FingerStick,
	TouchPad,
	TrackBall,
	FlightStick,
	GamePad,
	Unknown,
	SteeringWheel,
	HeadTracker,
	LightGun
};

class InputDevice {
protected:
	int axis_count;
	int button_count;
	int device_type;
	SubType sub_type;
	unsigned char flags;
	Button* buttons;
	int enabled;
public:
	// OFFSET: 0x0054e5e0, STATUS: COMPLETE
	inline InputDevice(int _device_type) {
		flags &= 0xc0;
		buttons = nullptr;
		axis_count = 0;
		button_count = 0;
		device_type = _device_type;
		sub_type = SubType::Invalid;
		enabled = 1;
	}

	RRE_DISABLE_COPY(InputDevice);
	
	virtual void RemoveAxis() = 0;

	// OFFSET: 0x004288c0, STATUS: COMPLETE
	inline virtual void SetEnabled(int _enabled) {
		enabled = _enabled;
	}

	// OFFSET: 0x0054e610, STATUS: COMPLETE
	inline virtual ~InputDevice() {
		if (buttons != nullptr) {
			delete[] buttons;
			buttons = nullptr;
		}
	}
};