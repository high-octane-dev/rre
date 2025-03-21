#pragma once
#include "gfx/x360_full_screen_render_pass.hpp"
#include "gfx/x360_shader.hpp"
#include "loading_icon.hpp"

class X360LoadingIcon : public LoadingIcon, public X360FullScreenRenderPass {
public:
	X360TextureMap* loading_screen;
	HANDLE thread_handle;
	HANDLE duplicate_proc_handle;
	HANDLE present_frame_lock;
	RTL_CRITICAL_SECTION lock;
	X360TextureMap* footer_bar;
	X360TextureMap* font_texture;
	struct ScreenTextFont* franklin_cond_stroke;
	struct DynamicTextIcon* text_icons[2];
	X360VertexShader* icon_vertex;
	X360PixelShader* icon_pixel;
	float unk_interval;
	unsigned int footer_state;
	int loading_tip_index;
public:
	X360LoadingIcon();
	virtual ~X360LoadingIcon() override;
	virtual void Enable() override;
	virtual void Disable() override;
	virtual void SetAnimationLocation(char*, float, float) override;
	virtual void SetAnimationSize(char*, float, float) override;
	virtual void SetAnimationOpacity(char*, float) override;
	virtual void SetAnimationColor(char*, unsigned int) override;
	virtual void Update() override;
	virtual void RenderIcon(LoadingIconAnim*, LoadingImageIcon*, float, float, float, float, unsigned int) override;
	virtual int Load(ParameterBlock*) override;

	virtual void Draw(int index) override;

	void InitLoadingTips(unsigned int, char*);
	void InitFooterResources();
	void RenderAnimation(LoadingIconAnim*);
	void SetRenderStates();
	void UpdateAnimationState(LoadingIconAnim*);
};