#include "music.hpp"

// OFFSET: 0x005bf160, STATUS: TODO
Music::~Music() {
}

// OFFSET: 0x0041b120, STATUS: TODO
X360Music::~X360Music() {
}

// OFFSET: 0x0041baa0, STATUS: TODO
int X360Music::Tick(float dt) {
    return 0;
}

// OFFSET: 0x0041b290, STATUS: TODO
int X360Music::Pause(int paused) {
    return 0;
}

// OFFSET: 0x0041c020, STATUS: TODO
void X360Music::CreateNumTracks(int param_1, int track_count) {
}

// OFFSET: 0x0041b340, STATUS: TODO
void X360Music::Play(int song, int param_3) {
}

// OFFSET: 0x004193b0, STATUS: TODO
bool X360Music::IsPlaying() {
    return false;
}

// OFFSET: 0x0041b380, STATUS: TODO
void X360Music::Repeat(int param_1, int param_2) {
}

// OFFSET: 0x0041b3c0, STATUS: TODO
void X360Music::Loop(int param_1, int param_2) {
}

// OFFSET: 0x0041a8c0, STATUS: TODO
bool X360Music::IsPaused() {
    return false;
}

// OFFSET: 0x0041a900, STATUS: TODO
int X360Music::Stop(float param_1) {
    return 0;
}

// OFFSET: 0x0041a8e0, STATUS: TODO
int X360Music::SetVolume(int param_1) {
    return 0;
}

// OFFSET: 0x0041a9c0, STATUS: TODO
int X360Music::RampVolume(int param_1, int param_2, int param_3) {
    return 0;
}

// OFFSET: 0x0041a8d0, STATUS: TODO
int X360Music::GetCurrentTrack() {
    return 0;
}

// OFFSET: 0x0041c0e0, STATUS: TODO
int X360Music::Shuffle(int param_1) {
    return 0;
}

// OFFSET: 0x0041c1d0, STATUS: TODO
int X360Music::ShuffleStartingAt(int param_1, int param_2) {
    return 0;
}

// OFFSET: 0x0041a9b0, STATUS: TODO
float X360Music::GetCurrentElapsedTime() {
    return 0.0f;
}

// OFFSET: INLINE, STATUS: TODO
int X360Music::GetAdditionalNumberOfJukeboxTracks() {
    return 0;
}