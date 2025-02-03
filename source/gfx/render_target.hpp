#pragma once
#include "math/frect.hpp"
#include "x360_texture_map.hpp"
#include "video_card.hpp"
#include "camera.hpp"
#include "image_data_format.hpp"

class RenderTarget {
protected:
    Camera* camera;
    int width;
    int height;
    int unk_camera_related;
    int frame_count;
    int unused;
    int unused2;
    ImageDataFormat unk_pixel_format;
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
    virtual void Create(VideoCard* unused) = 0;
    virtual void SetDimensions(VideoCard* unused) = 0;
    virtual void ApplyViewport() = 0;
    virtual void DrawCursor() = 0;
    virtual int Blt(FRECT* dest, TextureMap* texture, FRECT* unused0, unsigned int unused1, int use_linear, int alpha_blend, D3DCOLOR color) = 0;
    virtual int Unk6(int, int, int) = 0;
    virtual int Unk7(int) = 0;
    virtual void Unk8(int) = 0;
    virtual void Unk9();
    virtual int Unk10(int, int) = 0;
    virtual bool CheckDeviceFormat(ImageDataFormat) = 0;
    virtual int SetViewport(D3DVIEWPORT9*) = 0;
    virtual void SetFogDistances(float min, float max);
};