#include "cars2_event_database.hpp"

Cars2EventDatabase* lpGlobalEventDatabase = nullptr;

// OFFSET: INLINE, STATUS: COMPLETE
Cars2EventDatabase::Cars2EventDatabase() {
    event_sets = nullptr;
    event_sets_len = 0;
    string_block_allocator = nullptr;
    lpGlobalEventDatabase = this;
}

// OFFSET: 0x004b1070, STATUS: COMPLETE
Cars2EventDatabase::~Cars2EventDatabase() {
    if (event_sets != nullptr) {
        delete[] event_sets;
        event_sets = nullptr;
    }
    if (string_block_allocator != nullptr) {
        delete string_block_allocator;
        string_block_allocator = nullptr;
    }
    lpGlobalEventDatabase = nullptr;
}

// OFFSET: 0x00473a70, STATUS: TODO
void Cars2EventDatabase::Create() {
}

// OFFSET: 0x004294a0, STATUS: COMPLETE
Cars2EventInfo* Cars2EventDatabase::GetEventInfo(Cars2ActivityInfo* activity) {
    for (int i = 0; i < event_sets_len; i++) {
        Cars2EventSet* event_set = &event_sets[i];
        for (int j = 0; j < event_set->events_len; j++) {
            if (event_set->events[j].activity_info == activity) {
                return &event_set->events[j];
            }
        }
    }
    return nullptr;
}

// OFFSET: 0x00429460, STATUS: COMPLETE
Cars2EventInfo* Cars2EventDatabase::GetEventInfo(const char* name) {
    for (int i = 0; i < event_sets_len; i++) {
        Cars2EventInfo* event = event_sets[i].GetEventInfo(name);
        if (event != nullptr) {
            return event;
        }
    }
    return nullptr;
}

// OFFSET: 0x00429510, STATUS: COMPLETE
Cars2EventSet* Cars2EventDatabase::GetEventSet(Cars2EventInfo* info) {
    for (int i = 0; i < event_sets_len; i++) {
        if (event_sets[i].GetEventInfo(info->activity_name) != nullptr) {
            return &event_sets[i];
        }
    }
    return nullptr;
}

// OFFSET: 0x00429410, STATUS: TODO
Cars2EventSet* Cars2EventDatabase::GetEventSet(const char* name) {
    for (int i = 0; i < event_sets_len; i++) {
        if (_stricmp(name, event_sets[i].event_set_name) == 0) {
            return &event_sets[i];
        }
    }
    return nullptr;
}

// OFFSET: 0x004293b0, STATUS: COMPLETE
void Cars2EventDatabase::Reset() {
    for (int i = 0; i < event_sets_len; i++) {
        for (int j = 0; j < event_sets[i].events_len; j++) {
            event_sets[i].events[j].status = event_sets[i].events[j].default_status;
            event_sets[i].events[j].flags = 0;
        }
    }
}
