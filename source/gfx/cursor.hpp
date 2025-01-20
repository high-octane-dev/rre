#pragma once
#include "x360_texture_map.hpp"

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
	Cursor();
	~Cursor();
	Cursor(const Cursor&) = delete;
	Cursor& operator=(const Cursor&) = delete;
	void Draw();
};

extern Cursor* g_lpCursor;