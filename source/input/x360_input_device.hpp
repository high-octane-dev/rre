#pragma once
#include <Windows.h>
#include <dinput.h>
#include "input_device.hpp"
auto s = GUID_ConstantForce;

class X360InputDevice : public InputDevice {
protected:
	DIDEVICEINSTANCE device_info;
	int some_bool;
	IDirectInputDevice8A* device;
public:
	// OFFSET: 0x0041ead0, STATUS: COMPLETE
	inline X360InputDevice(int _device_type) : InputDevice(_device_type) {
		device = nullptr;
		memset(&device_info, 0, sizeof(DIDEVICEINSTANCE));
		some_bool = 0;
	}

	// OFFSET: 0x0041eb10, STATUS: COMPLETE
	inline virtual ~X360InputDevice() override {
		if (device != nullptr) {
			device->Unacquire();
			if (device->Release() == 0) {
				device = nullptr;
			}
		}
	}
};