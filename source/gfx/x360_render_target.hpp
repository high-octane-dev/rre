#pragma once

#include <d3d9.h>
#include "render_target.hpp"
#include "x360_shader.hpp"

using RBVIEWPORT = D3DVIEWPORT9;

class X360RenderTarget : public RenderTarget {
private:
    IUnknown* unk_array_1[3];
    IUnknown* unk_array_2[2];
    IUnknown* unk;
    int unused8;
    int unused9;
    int unused10;
    IDirect3DSurface9* depth_stencil_surface;
    IDirect3DTexture9* unk_texture;
    int unused11;
    int unused12;
    X360VertexShader* shadow_vertex;
    X360PixelShader* shadow_pixel;
    X360VertexShader* shadow_skinned_vertex;
    X360VertexShader* shadow_puppet_vertex;
    int unk_viewport_index;
    RBVIEWPORT viewport;
    struct Camera* used_camera;
    int unk_filter_index;
    int unused13;
    class X360VertexBufferManager* vertex_buffer_manager;
    class X360IndexBufferManager* index_buffer_manager;
    class X360VertexShaderManager* vertex_shader_manager;
    class X360PixelShaderManager* pixel_shader_manager;
    float fog_parameters[4];
    X360VertexShader* blt_vertex;
    X360PixelShader* blt_pixel;
    int texcoord_indices[4];
    int unused14;
    IUnknown* query;
    bool device_lost;
public:
	X360RenderTarget();
    void ApplyViewportImpl();
    bool CheckDeviceFormatImpl(D3DFORMAT);
    void Clear();
    int DrawCursorImpl();
    int DrawFullscreenEffects();
    int EndFrame();
    void IncrementFrameCount();
    D3DFORMAT MapToD3DFormat(UnkPixelFormat);
    int Recreate();
    void Reset();
    void SetCamera(Camera*);
    void SetUnkViewportIndex(int);
    bool StartFrame();

	virtual ~X360RenderTarget() override;
    virtual void Create(int unused) override;
    virtual void SetDimensions(int unused0, int unused1) override;
    virtual void ApplyViewport() override;
    virtual void DrawCursor() override;
    virtual void Blt(unsigned int unused, TextureMap* tex, int alpha_blend, D3DCOLOR color) override;
    virtual int Unk6(int, int, int) override;
    virtual int Unk7(int) override;
    virtual void Unk8(int) override;
    virtual void Unk9() override;
    virtual int Unk10(int, int) override;
    virtual bool CheckDeviceFormat(UnkPixelFormat) override;
    virtual int SetViewport(D3DVIEWPORT9*) override;
    virtual void SetFogDistances(float min, float max) override;
};

// extern X360RenderTarget* g_RenderTarget;