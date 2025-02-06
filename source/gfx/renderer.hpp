#pragma once
#include "x360_full_screen_render_pass.hpp"

namespace Renderer {
	struct RenderFrameData {
		int total_frames_prepared;
		int should_unk;
		int next_camera;
		int camera_indices[4];
		X360FullScreenRenderPass* fullscreen_render_pass;
	};
	extern RenderFrameData g_RenderFrameData[1];
	extern int s_NextFrameBufferToPrepare;
	extern int g_X360TotalFramesPrepared;

	void CompleteNextRenderFrame();
	void InitRenderer();
	int PrepareNextRenderFrame();
	int PrepareNextRenderDrawList(int);
	void UNK_00413690();
};