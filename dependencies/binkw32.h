#pragma once
#include <cstdint>
struct BINK;

extern "C" __declspec(dllimport) void __stdcall BinkClose(BINK* bink);
extern "C" __declspec(dllimport) std::int32_t __stdcall BinkCopyToBufferRect(BINK* bink, void* dst, std::int32_t dst_pitch, std::uint32_t dst_height, std::uint32_t dst_x, std::uint32_t dst_y, std::uint32_t src_x, std::uint32_t src_y, std::uint32_t src_w, std::uint32_t src_h, std::uint32_t flags);
extern "C" __declspec(dllimport) std::int32_t __stdcall BinkDoFrame(BINK* bink);
extern "C" __declspec(dllimport) std::int32_t __stdcall BinkGetRects(BINK* bink, std::uint32_t flags);
extern "C" __declspec(dllimport) void __stdcall BinkNextFrame(BINK* bink);
extern "C" __declspec(dllimport) BINK* __stdcall BinkOpen(const char* path, std::uint32_t flags);
extern "C" __declspec(dllimport) void* __stdcall BinkOpenDirectSound(std::uint32_t param);
extern "C" __declspec(dllimport) std::int32_t __stdcall BinkPause(BINK* bink, std::int32_t pause);
extern "C" __declspec(dllimport) std::int32_t __stdcall BinkSetSoundSystem(void* open, std::uint32_t param);
extern "C" __declspec(dllimport) void __stdcall BinkSetSoundTrack(std::uint32_t total_tracks, std::uint32_t* tracks);
extern "C" __declspec(dllimport) void __stdcall BinkSetVolume(BINK* bink, std::uint32_t track_id, std::int32_t volume);
extern "C" __declspec(dllimport) std::int32_t __stdcall BinkShouldSkip(BINK* bink);
extern "C" __declspec(dllimport) std::int32_t __stdcall BinkWait(BINK* bink);