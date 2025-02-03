#pragma once
#include <d3d9.h>
#include "video_card.hpp"

class X360VideoCard : public VideoCard {
private:
	IUnknown* unused;
public:
	D3DCAPS9 capabilities;
private:
	int current_frame;
	std::uint8_t flags;
	bool enable_fullscreen_effects;
public:
	X360VideoCard();
	void Create();
	bool IsFXEnabled();
	virtual ~X360VideoCard() override;
	virtual void FreeCursor() override;
	virtual int ResetStateManager() override;
	virtual int DisplayToScreen(int inc_frame_count) override;
	virtual void Unk4() override;
	virtual void Unk5() override;
	virtual void Unk6() override;
	virtual void Unk7() override;
	virtual void Unk8() override;
};

extern X360VideoCard* g_VideoCard;
extern IDirect3DDevice9* g_D3DDevice9;