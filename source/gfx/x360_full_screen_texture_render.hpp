#pragma once
#include "x360_texture_map.hpp"
#include "x360_full_screen_render_pass.hpp"
#include "util/macros.hpp"

/*
ok, so i've labelled this `textures` member as a single-element array because, for some inexplicable reason
they treat it as an array in both SetTexture and Draw. but, since we KNOW the maximum size of the struct,
it *cant* be an actual array and is likely a single-element one...

also if you hook those functions and print out the index youll see they never pass anything but 0 anyway so
*/
class X360FullScreenTextureRender : public X360FullScreenRenderPass {
private:
	TextureMap* textures[1];
public:
	RRE_DISABLE_COPY(X360FullScreenTextureRender);
	X360FullScreenTextureRender();
	~X360FullScreenTextureRender();
	virtual void Draw(int index) override;
	void SetTexture(int index, TextureMap* texture);
};

extern X360FullScreenTextureRender g_FullscreenRender;