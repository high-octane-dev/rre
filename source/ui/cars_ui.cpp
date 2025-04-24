#include "cars_ui.hpp"

// OFFSET: INLINE, STATUS: COMPLETE
CarsUI::CarsUI() : GameObject(1) {
    manager = nullptr;
    unused0 = -1;
    unused1 = 0;
    unused2 = 0;
    unused3 = -1;
}

// OFFSET: INLINE, STATUS: COMPLETE
CarsUI::~CarsUI() {
}

// OFFSET: 0x0050d340, STATUS: TODO
GameObject* CarsUI::Create() {
    /*
    manager = new CarsUIManager();
    manager->Create();
    AddChild(manager, -1);
    */
	return this;
}

// OFFSET: INLINE, STATUS: COMPLETE
int CarsUI::Tick(float dt) {
	return GameObject::Tick(dt);
}

// OFFSET: 0x004f8980, STATUS: TODO
void CarsUI::LoadCommon(int) {
}

// OFFSET: 0x004ceac0, STATUS: TODO
void CarsUI::LoadGroup(char*, char*, int) {
}

// OFFSET: 0x00495fb0, STATUS: TODO
void CarsUI::LoadLocalization() {
}
