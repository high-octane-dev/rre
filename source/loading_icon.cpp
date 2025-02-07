#include <cstdio>
#include <Windows.h>
#include "loading_icon.hpp"
#include "data_access.hpp"
#include "util/rsstring_util.hpp"

// OFFSET: INLINE, STATUS: TODO
LoadingImageIcon::LoadingImageIcon() {
}

// OFFSET: INLINE, STATUS: TODO
LoadingImageIcon::~LoadingImageIcon() {
}

// OFFSET: 0x005502e0, STATUS: TODO
LoadingIconAnim::LoadingIconAnim() {
	reference_anim = nullptr;
}

// OFFSET: INLINE, STATUS: COMPLETE
LoadingIconAnim::~LoadingIconAnim() {
	if (reference_anim != nullptr) {
		delete reference_anim;
	}
}


// OFFSET: 0x005503b0, STATUS: COMPLETE
LoadingIcon::LoadingIcon() {
	frame_count = 0;
	animation_count = 0;
	refresh_display = 1;
	should_lock = 0;
	frames = nullptr;
	animations = nullptr;
	unused_callback = nullptr;
	unused_callback_data = 0;
	enable_timestamp = 0;
}

// OFFSET: 0x00585de0, STATUS: TODO
LoadingIcon::~LoadingIcon() {
}

// OFFSET: 0x005ed180, STATUS: COMPLETE
void LoadingIcon::Create(char* name) {
	ParameterBlock icon_info{};
	char filename[260]{};

	RSStringUtil::Ssnprintf(filename, sizeof(filename), "%s.%s.lsi", name, "x360");
	unsigned int old_flag = lpDataAccess->flags & 0x2000;
	if (old_flag != 0) {
		lpDataAccess->flags = lpDataAccess->flags & 0xffffdfff;
	}
	lpDataAccess->LoadResourceFile(filename, 1, 0, -1, 0, 0, 0, 0, 0, 0);

	if (icon_info.OpenFile("icon_info.ini", 0, -1, nullptr, -1) != 0) {
		if (Load(&icon_info) != 0) {
			lpDataAccess->DropDevice(filename, 1);
			if (old_flag == 0) {
				if ((lpDataAccess->flags & 0x2000) == 0) {
					return;
				}
				lpDataAccess->flags = lpDataAccess->flags & 0xffffdfff;
			}
			else {
				if ((lpDataAccess->flags & 0x2000) != 0) {
					return;
				}
				lpDataAccess->flags = lpDataAccess->flags | 0x2000;
			}
		}
	}
}

// OFFSET: 0x005503e0, STATUS: COMPLETE
void LoadingIcon::Enable() {
	enable_timestamp = timeGetTime();
	should_lock = 1;
}

// OFFSET: 0x00550400, STATUS: COMPLETE
void LoadingIcon::Disable() {
	should_lock = 0;
	enable_timestamp = 0;
	unused_callback = nullptr;
	unused_callback_data = 0;
}

// OFFSET: 0x00550410, STATUS: TODO
void LoadingIcon::SetAnimationLocation(char*, float, float) {
}

// OFFSET: 0x00550490, STATUS: TODO
void LoadingIcon::SetAnimationSize(char*, float, float) {
}

// OFFSET: 0x00550510, STATUS: TODO
void LoadingIcon::SetAnimationOpacity(char*, float) {
}

// OFFSET: 0x00550580, STATUS: TODO
void LoadingIcon::SetAnimationColor(char*, unsigned int) {
}

// OFFSET: 0x00550a90, STATUS: COMPLETE
void LoadingIcon::Update() {
	float dt = std::fabsf(static_cast<float>(timeGetTime() - enable_timestamp));
	if (unused_callback != nullptr) {
		unused_callback(dt * 0.001f, unused_callback_data);
	}
}

// OFFSET: 0x005d6780, STATUS: COMPLETE
int LoadingIcon::Load(ParameterBlock* file) {
	if (file->ReadParameterBlock("General") != 0) {
		file->GetParameter("NumberOfFrames", 0, &frame_count);
		file->GetParameter("NumberOfAnimations", 0, &animation_count);
		file->GetParameter("RefreshDisplay", 1, &refresh_display);
		animations = new LoadingIconAnim[animation_count];
		frames = new LoadingImageIcon[frame_count];
		for (std::size_t i = 0; i < animation_count; i++) {
			char animation_block[260]{};
			RSStringUtil::Ssnprintf(animation_block, sizeof(animation_block), "Animation%d", i + 1);
			if (file->ReadParameterBlock(animation_block) == 0) {
				break;
			}
			LoadAnimation(file, &animations[i]);
		}
		return 1;
	}
	return 0;
}

// OFFSET: 0x005acdf0, STATUS: TODO
void LoadingIcon::LoadAnimation(ParameterBlock*, LoadingIconAnim*) {
}
