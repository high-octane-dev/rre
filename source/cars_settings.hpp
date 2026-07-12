#pragma once
#include "virtual_network.hpp"
#include "serializable_interface.hpp"
#include "cars2_launcher.hpp"

struct CarsSettings : public VNObject, public SerializableInterface {
    unsigned int game_mode;
    int difficulty;
    float difficulty_level;
    int time_of_day;
    unsigned char number_of_players;
    unsigned char number_of_ai;
    int viewport_type;
    unsigned int field11_0x28;
    int resolution_index;
    int field13_0x30;
    unsigned int field14_0x34;
    unsigned char field15_0x38;
    unsigned char field16_0x39;
    unsigned char field17_0x3a;
    unsigned char field18_0x3b;
    int enable_audio;
    int field20_0x40;
    unsigned char music_volume;
    unsigned char effects_volume;
    unsigned char effects_volume2;
    unsigned char dialogue_volume;
    unsigned char field25_0x48;
    unsigned char field26_0x49;
    char sound_id_map[16];
    unsigned int camera_recenter;
    unsigned int big_air;
    unsigned int field32_0x64;
    unsigned int tt_cam;
    int unk_player[2];
    unsigned int field35_0x74;
    int field36_0x78;
    int miles_or_kilometers;
    int miles_per_hour_or_kilometers_per_hour;
    ContainerList<void*> default_playlists[3];
    unsigned int map_layer_visible;
    int stats_layer_visible;
    unsigned int field42_0xd4;
    int enforce_free_camera_speed;
    struct CarsInfo* CarsInfo;
    unsigned int field45_0xe0;
    unsigned int field46_0xe4;
    int field47_0xe8;
    int anti_aliasing;
    int fullscreen_effects;
    unsigned int field50_0xf4;
    VehicleInformation vehicle_infos[2];
    unsigned int field52_0x1b8;
    VehicleInformation vehicle_infos2[6];
    int using_vehicle_infos2;
    char ai_names[256];
    int player_counters[2];
    int input_device_types[2];
    unsigned int field58_0x510;
    unsigned int field59_0x514;
    unsigned int start_on_title_screen_on_first_boot;
    unsigned char master_base_volume;
    unsigned char music_base_volume;
    unsigned char sfx_base_volume;
    unsigned char engine_base_volume;
    unsigned char dialogue_base_volume;
};
