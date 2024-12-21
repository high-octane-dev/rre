#include "gfx/video_card.hpp"

// OFFSET: 0x0059ca70
VideoCard::VideoCard()
{
	initialized = 0;
	unused = 0;
	should_inc_frame_count = 0;
	unused2 = 0;
	unused3 = 0;
	unused4.CLNonMacroCreate(1, 1, std::numeric_limits<int>::max());
	unk_refresh_rate_param = 1;
	unk_refresh_rate_param_2 = 1;
}

// OFFSET: INLINE
VideoCard::~VideoCard()
{
}

// OFFSET: INLINE
void VideoCard::FreeCursor()
{
}

// OFFSET: INLINE
void VideoCard::Unk8()
{
}
