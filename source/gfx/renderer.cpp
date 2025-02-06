#include <Windows.h>
#include "renderer.hpp"

Renderer::RenderFrameData Renderer::g_RenderFrameData[1] = {};
int Renderer::s_NextFrameBufferToPrepare = 0;
int Renderer::g_X360TotalFramesPrepared = 0;

// OFFSET: 0x00413670, STATUS: COMPLETE
void Renderer::CompleteNextRenderFrame() {
    s_NextFrameBufferToPrepare = s_NextFrameBufferToPrepare + 1;
    if (s_NextFrameBufferToPrepare != 0) {
        s_NextFrameBufferToPrepare = 0;
    }
}

// OFFSET: 0x004134e0, STATUS: WIP
void Renderer::InitRenderer() {
    // g_UnkIndex2 = 0;
    s_NextFrameBufferToPrepare = 0;
    g_RenderFrameData[0].total_frames_prepared = 0;
    g_RenderFrameData[0].next_camera = 0;
    g_RenderFrameData[0].camera_indices[0] = 0;
    g_RenderFrameData[0].should_unk = 0;
    g_RenderFrameData[0].fullscreen_render_pass = NULL;
    g_RenderFrameData[0].camera_indices[1] = 1;
    g_RenderFrameData[0].camera_indices[2] = 2;
    g_RenderFrameData[0].camera_indices[3] = 3;
    /*
    InitializeCriticalSection(&lpCriticalSection_006fd398);
    InitializeCriticalSection(&lpCriticalSection_006fd3b0);
    InitializeCriticalSection(&lpCriticalSection_006fd360);
    InitializeCriticalSection(&lpCriticalSection_006fd348);
    g_Viewports[0].X = PlatformGraphics_BackBufferViewport_OffsetX;
    g_Viewports[0].Y = PlatformGraphics_BackBufferViewport_OffsetY;
    g_Viewports[0].MinZ = 0.0;
    g_Viewports[0].MaxZ = 1.0;
    g_Viewports[0].Width = g_CameraWidth;
    g_Viewports[0].Height = g_CameraHeight;

    g_Viewports[1].X = PlatformGraphics_BackBufferViewport_OffsetX;
    g_Viewports[1].Y = PlatformGraphics_BackBufferViewport_OffsetY;
    g_Viewports[1].MinZ = 0.0;
    g_Viewports[1].MaxZ = 1.0;
    g_Viewports[1].Width = g_CameraWidth;
    g_Viewports[1].Height = g_CameraHeight;

    g_Viewports[2].X = PlatformGraphics_BackBufferViewport_OffsetX;
    g_Viewports[2].Y = PlatformGraphics_BackBufferViewport_OffsetY;
    g_Viewports[2].MinZ = 0.0;
    g_Viewports[2].MaxZ = 1.0;
    g_Viewports[2].Width = g_CameraWidth;
    g_Viewports[2].Height = g_CameraHeight;

    g_Viewports[3].X = PlatformGraphics_BackBufferViewport_OffsetX;
    g_Viewports[3].Y = PlatformGraphics_BackBufferViewport_OffsetY;
    g_Viewports[3].MinZ = 0.0;
    g_Viewports[3].MaxZ = 1.0;
    g_Viewports[3].Height = g_CameraHeight;
    g_Viewports[3].Width = g_CameraWidth;
    */
}

// OFFSET: 0x00413600, STATUS: COMPLETE
int Renderer::PrepareNextRenderFrame() {
    g_RenderFrameData[0].total_frames_prepared = g_X360TotalFramesPrepared;
    g_RenderFrameData[0].should_unk = 0;
    g_RenderFrameData[0].next_camera = 0;
    g_RenderFrameData[0].fullscreen_render_pass = nullptr;
    return 0;
}

// OFFSET: 0x00413620, STATUS: WIP
int Renderer::PrepareNextRenderDrawList(int index) {
    int next_camera_index = g_RenderFrameData[index].camera_indices[g_RenderFrameData[index].next_camera];
    g_RenderFrameData[index].next_camera = g_RenderFrameData[index].next_camera + 1;
    /*
    if (g_lpSurfaceShaderList != NULL) {
        BlockAllocator::Purge(g_lpSurfaceShaderList->shaders[next_camera_index].block_allocator);
        g_lpSurfaceShaderList->shaders[next_camera_index].next_index = g_lpSurfaceShaderList->shaders[next_camera_index].next_index + 1;
    }
    */
    return 0;
}

// OFFSET: 0x00413690, STATUS: WIP
void Renderer::UNK_00413690() {
    // _DAT_006fd3d4 = g_UnkIndex2;
    Sleep(0);
}
