#pragma once
#include <d3d9.h>
#include "x360_full_screen_render_pass.hpp"

namespace Renderer {
	struct RenderFrameData {
		int total_frames_prepared;
		int should_unk;
		int next_camera;
		int camera_indices[4];
		X360FullScreenRenderPass* fullscreen_render_pass;
	};

	extern RenderFrameData g_RenderFrameData[1]; // This array has 2 elements in Xbox 360, 1 on PC.
	// extern RTL_CRITICAL_SECTION g_UnusedRenderFrameDataLock; // Used in Xbox 360, unused on PC.
	// extern RTL_CRITICAL_SECTION g_UnusedSurfaceShaderListLock; // Used in Xbox 360, unused on PC.

	extern int s_NextFrameBufferToPrepare;
	extern int g_X360TotalFramesPrepared;
	extern D3DVIEWPORT9 g_Viewports[4];

	// extern RTL_CRITICAL_SECTION g_UnusedPresentFrameLock; // Used in Xbox 360, unused on PC.
	// extern RTL_CRITICAL_SECTION g_UnusedUnkLock; // Used in Xbox 360, unused on PC.

	void CompleteNextRenderFrame();
	void InitRenderer();
	int PrepareNextRenderFrame();
	int PrepareNextRenderDrawList(int);
	void UNK_00413690();
};