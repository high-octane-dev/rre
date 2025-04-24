#include "cars2_event_info.hpp"
#include "allocators/string_block_allocator.hpp"
#include "cars2_event_database.hpp"
#include <cars2_activity_database.hpp>

// OFFSET: 0x00428b60, STATUS: COMPLETE
Cars2EventInfo::Cars2EventInfo() {
	activity_name = nullptr;
	default_status = 0;
	status = 0;
	flags = 0;
	activity_info = nullptr;
	name_string_id = nullptr;
	frontend_preview_image_material = nullptr;
	ingame_preview_image_material = nullptr;
	event_set = nullptr;
	unk = 0;
}

// OFFSET: 0x00428b90, STATUS: COMPLETE
Cars2EventInfo::~Cars2EventInfo() {
	if (activity_name != nullptr) {
		lpGlobalEventDatabase->string_block_allocator->FreeString(activity_name);
	}
	if (this->name_string_id != nullptr) {
		lpGlobalEventDatabase->string_block_allocator->FreeString(name_string_id);
	}
	if (this->frontend_preview_image_material != nullptr) {
		lpGlobalEventDatabase->string_block_allocator->FreeString(frontend_preview_image_material);
	}
	if (this->ingame_preview_image_material != nullptr) {
		lpGlobalEventDatabase->string_block_allocator->FreeString(ingame_preview_image_material);
	}
}

// OFFSET: 0x00428c00, STATUS: COMPLETE
void Cars2EventInfo::Create(ParameterBlock* file) {
    char read_parameter[64]{};
    file->ReadParameterBlock(activity_name);
    file->GetParameter("Activity", activity_name, read_parameter, sizeof(read_parameter));
    activity_info = lpGlobalActivityDatabase->GetActivityInfo(read_parameter);
    file->GetParameter("Status", "Locked", read_parameter, sizeof(read_parameter));
    if (_stricmp(read_parameter, "Locked") == 0) {
        default_status = 1;
        status = 1;
    }
    else if (_stricmp(read_parameter, "Unlocked") == 0) {
        default_status = 2;
        status = 2;
    }
    else {
        default_status = 0;
        status = 0;
    }

    file->GetParameter("NameStringID", "", read_parameter, sizeof(read_parameter));
    name_string_id = lpGlobalEventDatabase->string_block_allocator->StringBlockAllocator_AllocStringByString(read_parameter, 0);
    strncpy(name_string_id, read_parameter, strlen(read_parameter) + 1);

    file->GetParameter("FrontendPreviewImageMaterial", "", read_parameter, sizeof(read_parameter));
    frontend_preview_image_material = lpGlobalEventDatabase->string_block_allocator->StringBlockAllocator_AllocStringByString(read_parameter, 0);
    strncpy(frontend_preview_image_material, read_parameter, strlen(read_parameter) + 1);

    file->GetParameter("IngamePreviewImageMaterial", "", read_parameter, sizeof(read_parameter));
    ingame_preview_image_material = lpGlobalEventDatabase->string_block_allocator->StringBlockAllocator_AllocStringByString(read_parameter, 0);
    strncpy(ingame_preview_image_material, read_parameter, strlen(read_parameter) + 1);
}

// OFFSET: 0x00428e20, STATUS: COMPLETE
void Cars2EventInfo::SetFlags(int _flags) {
	if (flags < _flags) {
		flags = _flags;
	}
}

// OFFSET: 0x00428e30, STATUS: COMPLETE
int Cars2EventInfo::Serialize(void* buffer, int len) {
	*reinterpret_cast<int*>(buffer) = status;
	*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(int)) = flags;
	return sizeof(int) * 2;
}

// OFFSET: 0x00428e50, STATUS: COMPLETE
int Cars2EventInfo::DeSerialize(void* buffer, int len) {
	status = *reinterpret_cast<int*>(buffer);
	flags = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(int));
	return sizeof(int) * 2;
}

// OFFSET: INLINE, STATUS: COMPLETE
int Cars2EventInfo::GetSerializedDataSize() {
	return 0;
}