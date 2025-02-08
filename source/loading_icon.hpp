#pragma once
#include "parameter_block.hpp"
#include "gfx/x360_texture_map.hpp"

class LoadingImageIcon {
public:
    TextureMap* texture_map;
    unsigned int unk;
    unsigned int unk2;
    unsigned int unk3;
    unsigned int unk4;
public:
	LoadingImageIcon();
	LoadingImageIcon(const LoadingImageIcon&) = delete;
	LoadingImageIcon& operator=(const LoadingImageIcon&) = delete;
	~LoadingImageIcon();
};

class LoadingIconAnim {
public:
	char* reference_name;
    short start_frame;
    short end_frame;
    float start_pos_x;
    float start_pos_y;
    float end_pos_x;
    float end_pos_y;
    float start_size_x;
    float start_size_y;
    float end_size_x;
    float end_size_y;
    float start_opacity;
    float end_opacity;
    bool loop_anim;
    bool loop_movement;
    bool loop_sizing;
    bool loop_opacity;
    bool reversable_anim;
    bool reversable_movement;
    bool reversable_sizing;
    bool reversable_opacity;
    bool end_frame_lt_start;
    bool end_pos_x_lt_start;
    bool end_pos_y_lt_start;
    bool end_size_x_lt_start;
    bool end_size_y_lt_start;
    bool end_opacity_lt_start;
    bool enable_alpha_blending;
    unsigned short flags;
    float animate_time;
    float move_increment_x;
    float move_increment_y;
    float size_increment_x;
    float size_increment_y;
    float opacity_time;
    float start_frame_copy;
    float location_x;
    float location_y;
    float size_x;
    float size_y;
    float opacity;
    unsigned int color;
    unsigned int start_time_ms;
    bool color_set;
public:
	LoadingIconAnim();
	LoadingIconAnim(const LoadingIconAnim&) = delete;
	LoadingIconAnim& operator=(const LoadingIconAnim&) = delete;
	~LoadingIconAnim();
    
    void SetMoveTime(float);
    void SetSizeTime(float);
};

class LoadingIcon {
public:
	std::size_t frame_count;
	std::size_t animation_count;
	int refresh_display;
	int should_lock;
	LoadingIconAnim* animations;
	LoadingImageIcon* frames;
	void (*unused_callback)(float, unsigned int);
	unsigned int unused_callback_data;
	unsigned int enable_timestamp;
public:
	LoadingIcon();
	LoadingIcon(const LoadingIcon&) = delete;
	LoadingIcon& operator=(const LoadingIcon&) = delete;
	virtual ~LoadingIcon();
	virtual void Create(char*);
	virtual void Enable();
	virtual void Disable();
	virtual void SetAnimationLocation(char*, float, float);
	virtual void SetAnimationSize(char*, float, float);
	virtual void SetAnimationOpacity(char*, float);
	virtual void SetAnimationColor(char*, unsigned int);
	virtual void Update();
	virtual void RenderIcon(LoadingIconAnim*, LoadingImageIcon*, float, float, float, float, unsigned int) = 0;
	virtual int Load(ParameterBlock *);
	virtual void LoadAnimation(ParameterBlock*, LoadingIconAnim*);
};