#include "cars2_story_event.hpp"

// OFFSET: 0x0042a920, STATUS: COMPLETE
int Cars2StoryEvent::Serialize(void* buffer, int len) {
	*reinterpret_cast<int*>(buffer) = static_cast<int>(status);
	*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(int)) = bolt_banners[0].earned;
	*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(int) * 2) = bolt_banners[1].earned;
	*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(int) * 3) = bolt_banners[2].earned;
	return 16;
}

// OFFSET: 0x0042a950, STATUS: COMPLETE
int Cars2StoryEvent::DeSerialize(void* buffer, int len) {
	status = static_cast<StoryEventStatusType>(*reinterpret_cast<int*>(buffer));
	bolt_banners[0].earned = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(int));
	bolt_banners[1].earned = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(int) * 2);
	bolt_banners[2].earned = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(int) * 3);
	return 16;
}

// OFFSET: INLINE, STATUS: COMPLETE
int Cars2StoryEvent::GetSerializedDataSize() {
	return 0;
}