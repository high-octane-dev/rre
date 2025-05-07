#pragma once
#include "gfx/x360_texture_map.hpp"
#include "parameter_block.hpp"
#include "util/macros.hpp"

class LoadingImageIcon {
public:
    TextureMap* texture_map;
    unsigned int width;
    unsigned int height;
    D3DFORMAT format;
public:
    RRE_DISABLE_COPY(LoadingImageIcon);
	LoadingImageIcon();
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
    float current_frame;
    float location_x;
    float location_y;
    float size_x;
    float size_y;
    float opacity;
    unsigned int color;
    unsigned int start_time_ms;
    bool color_set;
public:
    RRE_DISABLE_COPY(LoadingIconAnim);
	LoadingIconAnim();
	~LoadingIconAnim();
    
    void SetMoveTime(float);
    void SetSizeTime(float);
};

class LoadingIcon {
public:
	std::size_t frame_count;
	std::size_t animation_count;
	int refresh_display;
	int enabled;
	LoadingIconAnim* animations;
	LoadingImageIcon* frames;
	void (*unused_callback)(float, unsigned int);
	unsigned int unused_callback_data;
	unsigned int enable_timestamp;
public:
    RRE_DISABLE_COPY(LoadingIcon);
	LoadingIcon();
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