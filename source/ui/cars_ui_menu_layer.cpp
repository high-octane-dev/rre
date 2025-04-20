#include "cars_ui_menu_layer.hpp"

// OFFSET: 0x004b0920, STATUS: COMPLETE
CarsUIMenuLayer::CarsUIMenuLayer() : CarsBaseLayer() {
	flags |= 0x1100;
	anim_state = 0;
	full_anim = nullptr;
	partial_anim = nullptr;
}

// OFFSET: 0x0x00453b60, STATUS: WIP
CarsUIMenuLayer::~CarsUIMenuLayer()
{
	// uintptr_t lpCarsUI = *reinterpret_cast<uintptr_t*>(0x006fe590);
	// uintptr_t ui_flash_animation_manager = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(lpCarsUI + 0x24) + 0x2c);

	if (full_anim != nullptr) {
		// ui_flash_animation_manager->Deregister(full_anim);
		delete[] full_anim;
		full_anim = nullptr;
	}
	if (partial_anim != nullptr) {
		// ui_flash_animation_manager->Deregister(partial_anim);
		delete[] partial_anim;
		partial_anim = nullptr;
	}
}

// OFFSET: 0x00453c30, STATUS: TODO
void CarsUIMenuLayer::CreateFromFile(char*, ParameterBlock*) {
}

// OFFSET: 0x00453d60, STATUS: TODO
void CarsUIMenuLayer::PlayAnim0(int is_full_on) {
}

// OFFSET: 0x00453da0, STATUS: TODO
void CarsUIMenuLayer::PlayAnim1(int is_full_on) {
}

// OFFSET: INLINE, STATUS: COMPLETE
void CarsUIMenuLayer::Unk1() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void CarsUIMenuLayer::Unk2() {
}

// OFFSET: 0x00496fe0, STATUS: TODO
void CarsUIMenuLayer::SetVisibleIfNotPlayingAnim1() {
}

// OFFSET: 0x00496fe0, STATUS: TODO
void CarsUIMenuLayer::SetVisibleIfNotPlayingAnim2() {
}

// OFFSET: INLINE, STATUS: COMPLETE
char* CarsUIMenuLayer::GetSelectedOption() {
	return nullptr;
}