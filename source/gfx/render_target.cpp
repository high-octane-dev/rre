#include "render_target.hpp"

// OFFSET: 0x0055ddd0, STATUS: COMPLETE
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

// OFFSET: 0x0055de00, STATUS: COMPLETE
RenderTarget::~RenderTarget() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void RenderTarget::Unk9() {
}

// OFFSET: 0x00413710, STATUS: COMPLETE
void RenderTarget::SetFogDistances(float min, float max) {
	maximum_fog_distance = max;
	minimum_fog_distance = min;
}
