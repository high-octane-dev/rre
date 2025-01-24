#include "cursor.hpp"

Cursor* g_lpCursor = nullptr;

// OFFSET: 0x004141b0, STATUS: COMPLETE
Cursor::Cursor() {
	unk_width = 64.0;
	unk_height = 64.0;
	wrench_inactive = nullptr;
	wrench_active = nullptr;
	wrench_clicked = nullptr;
	frame_timer = 0;
	cursor_clicked_state = 0;
}

// OFFSET: 0x004141d0, STATUS: COMPLETE
Cursor::~Cursor() {
	if (wrench_inactive != nullptr) {
		wrench_inactive->Release();
	}
	if (wrench_active != nullptr) {
		wrench_active->Release();
	}
	if (wrench_clicked != nullptr) {
		wrench_clicked->Release();
	}
}

// OFFSET: 0x00414740, STATUS: TODO
void Cursor::Draw() {
}
