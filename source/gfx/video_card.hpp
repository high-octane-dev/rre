#pragma once
#include "containers/container_list.hpp"
#include "util/macros.hpp"

class VideoCard {
public:
	int initialized;
protected:
	int unused;
	int should_inc_frame_count;
	int unk_refresh_rate_param;
	int unk_refresh_rate_param_2;
	int unused2;
	int unused3;
	ContainerList<std::uint32_t> unused4;
public:
	RRE_DISABLE_COPY(VideoCard);
	VideoCard();
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