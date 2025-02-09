#include <cstdio>
#include <Windows.h>
#include "loading_icon.hpp"
#include "data_access.hpp"
#include "util/rsstring_util.hpp"

// OFFSET: INLINE, STATUS: COMPLETE
LoadingImageIcon::LoadingImageIcon() {
	texture_map = nullptr;
}

// OFFSET: INLINE, STATUS: COMPLETE
LoadingImageIcon::~LoadingImageIcon() {
	if (texture_map != nullptr) {
		texture_map->Release();
		texture_map = nullptr;
		// this is not a typo they deadass do this twice
		texture_map = nullptr;
	}
}

// OFFSET: 0x005502e0, STATUS: COMPLETE
LoadingIconAnim::LoadingIconAnim() {
	reference_name = nullptr;
	color_set = false;
}

// OFFSET: INLINE, STATUS: COMPLETE
LoadingIconAnim::~LoadingIconAnim() {
	if (reference_name != nullptr) {
		delete[] reference_name;
	}
}

// OFFSET: 0x00550350, STATUS: COMPLETE
void LoadingIconAnim::SetMoveTime(float move_time) {
	if (0.0 < move_time) {
		flags = flags & 0xFFF9;
		float move_time_frame = (move_time * 1000.0f) / 17.0f;
		move_increment_x = (end_pos_x - start_pos_x) / move_time_frame;
		move_increment_y = (end_pos_y - start_pos_y) / move_time_frame;
		return;
	}
	flags = flags | 6;
	move_increment_x = 0.0;
	move_increment_y = 0.0;
}

// OFFSET: 0x005502f0, STATUS: COMPLETE
void LoadingIconAnim::SetSizeTime(float size_time) {
	if (0.0 < size_time) {
		flags = flags & 0xFFE7;
		float size_time_frame = (size_time * 1000.0f) / 17.0f;
		size_increment_x = (end_size_x - start_size_x) / size_time_frame;
		size_increment_y = (end_size_y - start_size_y) / size_time_frame;
		return;
	}
	flags = flags | 0x18;
	size_increment_x = 0.0;
	size_increment_y = 0.0;
}

// OFFSET: 0x005503b0, STATUS: COMPLETE
LoadingIcon::LoadingIcon() {
	frame_count = 0;
	animation_count = 0;
	refresh_display = 1;
	enabled = 0;
	frames = nullptr;
	animations = nullptr;
	unused_callback = nullptr;
	unused_callback_data = 0;
	enable_timestamp = 0;
}

// OFFSET: 0x00585de0, STATUS: COMPLETE
LoadingIcon::~LoadingIcon() {
	if (frames != nullptr) {
		delete[] frames;
	}
	if (animations != nullptr) {
		delete[] animations;
	}
}

// OFFSET: 0x005ed180, STATUS: COMPLETE
void LoadingIcon::Create(char* name) {
	ParameterBlock icon_info{};
	char filename[260]{};

	RSStringUtil::Ssnprintf(filename, sizeof(filename), "%s.%s.lsi", name, "x360");
	// Inlined call: lpDataAccess->TagForSave(0);
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
	enabled = 1;
}

// OFFSET: 0x00550400, STATUS: COMPLETE
void LoadingIcon::Disable() {
	enabled = 0;
	enable_timestamp = 0;
	unused_callback = nullptr;
	unused_callback_data = 0;
}

// OFFSET: 0x00550410, STATUS: COMPLETE
void LoadingIcon::SetAnimationLocation(char* name, float x, float y) {
	if (animation_count < 1) {
		return;
	}
	for (std::size_t i = 0; i < animation_count; i++) {
		if (animations[i].reference_name != nullptr) {
			if (strcmp(animations[i].reference_name, name) == 0) {
				animations[i].location_x = x;
				animations[i].location_y = x;
			}
		}
	}
	return;
}

// OFFSET: 0x00550490, STATUS: COMPLETE
void LoadingIcon::SetAnimationSize(char* name, float size_x, float size_y) {
	if (animation_count < 1) {
		return;
	}
	for (std::size_t i = 0; i < animation_count; i++) {
		if (animations[i].reference_name != nullptr) {
			if (strcmp(animations[i].reference_name, name) == 0) {
				animations[i].size_x = size_x;
				animations[i].size_y = size_y;
			}
		}
	}
	return;
}

// OFFSET: 0x00550510, STATUS: COMPLETE
void LoadingIcon::SetAnimationOpacity(char* name, float opacity) {
	if (animation_count < 1) {
		return;
	}
	for (std::size_t i = 0; i < animation_count; i++) {
		if (animations[i].reference_name != nullptr) {
			if (strcmp(animations[i].reference_name, name) == 0) {
				animations[i].opacity = opacity;
			}
		}
	}
	return;
}

// OFFSET: 0x00550580, STATUS: COMPLETE
void LoadingIcon::SetAnimationColor(char* name, unsigned int color) {
	if (animation_count < 1) {
		return;
	}
	for (std::size_t i = 0; i < animation_count; i++) {
		if (animations[i].reference_name != nullptr) {
			if (strcmp(animations[i].reference_name, name) == 0) {
				animations[i].color = color;
				animations[i].color_set = true;
			}
		}
	}
	return;
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

// OFFSET: 0x005acdf0, STATUS: COMPLETE
void LoadingIcon::LoadAnimation(ParameterBlock* file, LoadingIconAnim* anim) {
	char reference_name_string[260]{};
	if (file->GetParameter("ReferenceName", reference_name_string, sizeof(reference_name_string)) == 0) {
		anim->reference_name = nullptr;
	}
	else {
		std::size_t ref_len = strnlen_s(reference_name_string, sizeof(reference_name_string));
		anim->reference_name = new char[ref_len + 1] {};
		std::memcpy(anim->reference_name, reference_name_string, ref_len);
	}

	anim->flags = 0;
	anim->start_time_ms = 0;
	float start_time_sec = 0.0f;
	file->GetParameter("StartTime", 0.0f, &start_time_sec);
	if (start_time_sec > 0.0f) {
		anim->flags = 0x40;
		anim->start_time_ms = static_cast<unsigned int>(start_time_sec * 1000.0f);
	}

	int start_frame = 0;
	int end_frame = 0;
	file->GetParameter("StartFrame", 1, &start_frame);
	file->GetParameter("EndFrame", 1, &end_frame);
	anim->start_frame = end_frame - 1;
	if (start_frame - 1 < end_frame - 1) {
		anim->start_frame = start_frame - 1;
	}
	anim->end_frame = end_frame - 1;
	if (end_frame - 1 < start_frame - 1) {
		anim->end_frame = start_frame - 1;
	}
	anim->current_frame = static_cast<float>(std::abs(anim->start_frame));
	anim->end_frame_lt_start = end_frame < start_frame;

	int temp = 0;
	file->GetParameter("LoopAnimation", 0, &temp);
	anim->loop_anim = static_cast<bool>(temp);
	file->GetParameter("ReversableAnimation", 0, &temp);
	anim->reversable_anim = static_cast<bool>(temp);

	float animate_time = 0.0f;
	file->GetParameter("AnimateTime", 0.0f, &animate_time);
	if (animate_time <= 0.0f) {
		anim->animate_time = 0.0f;
	}
	else {
		anim->animate_time = static_cast<float>(std::abs(anim->end_frame - anim->start_frame)) / ((1000.0f * animate_time) / 17.0f);
	}

	float start_pos_x = 0.0f;
	float end_pos_x = 0.0f;
	file->GetParameter("StartPosX", 0.0f, &start_pos_x);
	file->GetParameter("EndPosX", 0.0f, &end_pos_x);
	anim->start_pos_x = end_pos_x;
	if (start_pos_x - end_pos_x < 0.0) {
		anim->start_pos_x = start_pos_x;
	}
	anim->end_pos_x = start_pos_x;
	if (start_pos_x - end_pos_x < 0.0) {
		anim->end_pos_x = end_pos_x;
	}
	anim->end_pos_x_lt_start = end_pos_x < start_pos_x;

	// WTFRB: wtf why is this handled differently...
	file->GetParameter("StartPosY", 0.0f, &anim->start_pos_y);
	file->GetParameter("EndPosY", 0.0f, &anim->end_pos_y);
	anim->end_pos_y_lt_start = anim->end_pos_y < anim->start_pos_y;

	anim->location_x = anim->start_pos_x;
	anim->location_y = anim->start_pos_y;

	file->GetParameter("LoopMovement", 0, &temp);
	anim->loop_movement = static_cast<bool>(temp);
	file->GetParameter("ReversableMovement", 0, &temp);
	anim->reversable_movement = static_cast<bool>(temp);

	float move_time = 0.0f;
	file->GetParameter("MoveTime", 0.0f, &move_time);
	anim->SetMoveTime(move_time);

	float start_size_x = 0.0f;
	float end_size_x = 0.0f;
	file->GetParameter("StartSizeX", 32.0f, &start_size_x);
	file->GetParameter("EndSizeX", 32.0f, &end_size_x);
	anim->start_size_x = end_size_x;
	if (start_size_x - end_size_x < 0.0) {
		anim->start_size_x = start_size_x;
	}
	anim->end_size_x = start_size_x;
	if (start_size_x - end_size_x < 0.0) {
		anim->end_size_x = start_size_x;
	}
	anim->end_size_x_lt_start = end_size_x < start_size_x;

	// WTFRB: wtf why is this handled differently...
	file->GetParameter("StartSizeY", 32.0f, &anim->start_size_y);
	file->GetParameter("EndSizeY", 32.0f, &anim->end_size_y);
	anim->end_size_y_lt_start = anim->end_size_y < anim->start_size_y;

	anim->size_x = anim->start_size_x;
	anim->size_y = anim->start_size_y;

	file->GetParameter("LoopSizing", 0, &temp);
	anim->loop_sizing = static_cast<bool>(temp);
	file->GetParameter("ReversableSizing", 0, &temp);
	anim->reversable_sizing = static_cast<bool>(temp);

	float size_time = 0.0f;
	file->GetParameter("SizeTime", 0.0f, &size_time);
	anim->SetSizeTime(size_time);

	float start_opacity = 0.0f;
	float end_opacity = 0.0f;
	file->GetParameter("StartOpacity", 100.0f, &start_opacity);
	file->GetParameter("EndOpacity", 100.0f, &end_opacity);
	start_opacity /= 100.0f;
	end_opacity /= 100.0f;
	anim->start_opacity = end_opacity;
	if (start_opacity - end_opacity < 0.0) {
		anim->start_opacity = start_opacity;
	}
	anim->end_opacity = start_opacity;
	if (start_opacity - end_opacity < 0.0) {
		anim->end_opacity = end_pos_x;
	}
	anim->end_opacity_lt_start = end_opacity < start_opacity;

	anim->opacity = anim->start_opacity;

	file->GetParameter("AlphaBlendingOn", 1, &temp);
	anim->enable_alpha_blending = static_cast<bool>(temp);
	file->GetParameter("LoopOpacity", 0, &temp);
	anim->loop_opacity = static_cast<bool>(temp);
	file->GetParameter("ReversableOpacity", 0, &temp);
	anim->reversable_opacity = static_cast<bool>(temp);

	float opacity_time = 0.0f;
	file->GetParameter("OpacityTime", 0.0f, &opacity_time);
	if (opacity_time <= 0.0f) {
		anim->opacity_time = 0.0f;
		anim->flags |= 32;
	}
	else {
		anim->opacity_time = static_cast<float>(std::abs(anim->end_opacity - anim->start_opacity)) / ((1000.0f * opacity_time) / 17.0f);
	}
}
