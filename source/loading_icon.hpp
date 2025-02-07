#pragma once
#include "parameter_block.hpp"

class LoadingImageIcon {
public:

public:
	LoadingImageIcon();
	LoadingImageIcon(const LoadingImageIcon&) = delete;
	LoadingImageIcon& operator=(const LoadingImageIcon&) = delete;
	~LoadingImageIcon();
};

class LoadingIconAnim {
public:
	char* reference_anim;
public:
	LoadingIconAnim();
	LoadingIconAnim(const LoadingIconAnim&) = delete;
	LoadingIconAnim& operator=(const LoadingIconAnim&) = delete;
	~LoadingIconAnim();
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
	virtual void LoadAnimation(ParameterBlock*, struct LoadingIconAnim*);
};