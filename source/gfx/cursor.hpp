#pragma once
#include "x360_texture_map.hpp"
#include "util/macros.hpp"

class Cursor {
public:
	float unk_width;
	float unk_height;
	X360TextureMap* wrench_inactive;
	X360TextureMap* wrench_clicked;
	X360TextureMap* wrench_active;
	unsigned int frame_timer;
	unsigned int cursor_clicked_state;
public:
	RRE_DISABLE_COPY(Cursor);
	Cursor();
	~Cursor();
	void Draw();
};

extern Cursor* g_lpCursor;