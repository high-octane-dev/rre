#pragma once
#include "x360_texture_map.hpp"

enum class UnkPixelFormat {
    Invalid = 0,
    A8 = 8,
    R5G6B5 = 565,
    A1R5G6B5 = 1565,
    A4R4G4B4 = 4444,
    A8R8G8B8 = 8888
};

class RenderTarget {
protected:
    struct Camera* camera;
    int width;
    int height;
    int unk_camera_related;
    int frame_count;
    int unused;
    int unused2;
    UnkPixelFormat unk_pixel_format;
    int fog_uninitialized;
    D3DCOLOR fog_color;
    int should_render_ui;
    int unused3;
    int unused4;
    int unused5;
    int unused6;
    int unused7;
    int unk_increment;
    float minimum_fog_distance;
    float maximum_fog_distance;
public:
	RenderTarget();
	RenderTarget(const RenderTarget&) = delete;
	RenderTarget& operator=(const RenderTarget&) = delete;

	virtual ~RenderTarget();
    virtual void Create(int unused) = 0;
    virtual void SetDimensions(int unused0, int unused1) = 0;
    virtual void ApplyViewport() = 0;
    virtual void DrawCursor() = 0;
    virtual void Blt(unsigned int unused, TextureMap* tex, int alpha_blend, D3DCOLOR color) = 0;
    virtual int Unk6(int, int, int) = 0;
    virtual int Unk7(int) = 0;
    virtual void Unk8(int) = 0;
    virtual void Unk9();
    virtual int Unk10(int, int) = 0;
    virtual bool CheckDeviceFormat(UnkPixelFormat) = 0;
    virtual int SetViewport(D3DVIEWPORT9*) = 0;
    virtual void SetFogDistances(float min, float max);
};