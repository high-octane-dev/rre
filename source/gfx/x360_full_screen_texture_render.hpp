#pragma once
#include "x360_texture_map.hpp"

/*
ok, so i've labelled this `textures` member as a single-element array because, for some inexplicable reason
they treat it as an array in both SetTexture and FUN_004134a0. but, since we KNOW the maximum size of the struct,
it *cant* be an actual array and is likely a single-element one...

also if you hook those functions and print out the index youll see they never pass anything but 0 anyway so
*/
class X360FullScreenTextureRender {
private:
	TextureMap* textures[1];
public:
	X360FullScreenTextureRender();
	~X360FullScreenTextureRender();
	X360FullScreenTextureRender(const X360FullScreenTextureRender&) = delete;
	X360FullScreenTextureRender& operator=(const X360FullScreenTextureRender&) = delete;
	virtual void FUN_004134a0(int index);
	void SetTexture(int index, TextureMap* texture);
};

extern X360FullScreenTextureRender g_FullscreenRender;