#pragma once
#include "../container_list.hpp"

class VideoCard {
protected:
	int initialized;
	int unused;
	int should_inc_frame_count;
	int unk_refresh_rate_param;
	int unk_refresh_rate_param_2;
	int unused2;
	int unused3;
	ContainerList<std::uint32_t> unused4;
public:
	VideoCard();
	VideoCard(const VideoCard&) = delete;
	VideoCard& operator=(const VideoCard&) = delete;
	virtual ~VideoCard();
	virtual void FreeCursor();
	virtual int ResetStateManager() = 0;
	virtual int DisplayToScreen(int inc_frame_count) = 0;
	virtual void Unk4() = 0;
	virtual void Unk5() = 0;
	virtual void Unk6() = 0;
	virtual void Unk7() = 0;
	virtual void Unk8();
};