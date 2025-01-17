#include "render_target.hpp"

// OFFSET: 0x0055ddd0
RenderTarget::RenderTarget() {
	camera = nullptr;
	should_render_ui = 0;
	unk_camera_related = 0;
	frame_count = 0;
	unk_pixel_format = UnkPixelFormat::Invalid;
	unused3 = 0;
	unused4 = 0;
	unused5 = 0;
	unused6 = 0;
}

// OFFSET: 0x0055de00
RenderTarget::~RenderTarget() {
}

// OFFSET: INLINE
void RenderTarget::Unk9() {
}

// OFFSET: 0x00413710
void RenderTarget::SetFogDistances(float min, float max) {
	maximum_fog_distance = max;
	minimum_fog_distance = min;
}
