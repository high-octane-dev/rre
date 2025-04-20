#pragma once

struct _StreamingSectionCallbackInfo {
	char* unk1;
	char* unk2;
	unsigned int unk3;
	unsigned int unk4;
	unsigned int unk5;
	char* unk6;
	char* unk7;
};

class StreamingListener {
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual void SectionArriving(_StreamingSectionCallbackInfo const*) {}
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual void SectionArrived(_StreamingSectionCallbackInfo const*) {}
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual void SectionLeaving(_StreamingSectionCallbackInfo const*) {}
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual void UserDataArriving(_StreamingSectionCallbackInfo const*) {}
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual void UserDataLeaving(_StreamingSectionCallbackInfo const*) {}
};