#include "video_card.hpp"

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

VideoCard::~VideoCard()
{
}

void VideoCard::FreeCursor()
{
}

void VideoCard::Unk8()
{
}
