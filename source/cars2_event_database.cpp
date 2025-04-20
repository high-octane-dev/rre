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

// OFFSET: 0x004294a0, STATUS: TODO
Cars2EventInfo* Cars2EventDatabase::GetEventInfo(Cars2ActivityInfo* activity) {
    return nullptr;
}

// OFFSET: 0x00429460, STATUS: TODO
Cars2EventInfo* Cars2EventDatabase::GetEventInfo(char* name) {
    return nullptr;
}

// OFFSET: 0x00429510, STATUS: TODO
Cars2EventSet* Cars2EventDatabase::GetEventSet(Cars2EventInfo* info) {
    return nullptr;
}

// OFFSET: 0x00429410, STATUS: TODO
Cars2EventSet* Cars2EventDatabase::GetEventSet(char* name) {
    return nullptr;
}

// OFFSET: 0x004293b0, STATUS: TODO
void Cars2EventDatabase::Reset() {
}
