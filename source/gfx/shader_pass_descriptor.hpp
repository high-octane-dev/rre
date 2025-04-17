#pragma once
#include "base_object.hpp"
#include "x360_shader_parameters_data_item.hpp"

struct ShaderPassAnimationData;

class ShaderPassDescriptor : BaseObject {
public:
	unsigned char texture_map_count;
	unsigned short texture_map_string_table_indices[16];
	unsigned int shader_parameter_count;
	ShaderPassAnimationData* animation_data;
	ShaderPassAnimationData** animation_data_list;
	unsigned int is_2d;
	unsigned int dependent_material_string_table_indices[2];
	float material_pass_start;
	float material_pass_end;
	unsigned int force_unique;
	char texture_coordinate_indices[16];
public:
	ShaderPassDescriptor();
	ShaderPassDescriptor(const ShaderPassDescriptor&) = delete;
	ShaderPassDescriptor& operator=(const ShaderPassDescriptor&) = delete;

	virtual ~ShaderPassDescriptor() override;
	virtual int Unk0(char*) = 0;
	virtual int Unk1(int) = 0;
	virtual void Unk2() = 0;
	virtual int GetTextureStringTableIndex(int index) = 0;
};

struct ShaderPassSamplerDescEntry {
	unsigned int sampler;
	D3DSAMPLERSTATETYPE type;
	unsigned int value;
};

struct ShaderPassRenderStateDescEntry {
	D3DRENDERSTATETYPE type;
	unsigned int value;
};

class X360ShaderPassDescriptor : public ShaderPassDescriptor {
public:
	unsigned char flags;
	short vertex_shader_index;
	short pixel_shader_index;
	unsigned int sampler_parameter_count;
	unsigned int render_state_parameter_count;
	unsigned int unused0;
	unsigned int unused1;
	int camera_string_table_index;
	int material_index;
	float gloss_power;
	ShaderPassSamplerDescEntry* sampler_state_descriptor;
	ShaderPassRenderStateDescEntry* render_state_descriptor;
	X360ShaderParametersDataItem* shader_parameters_data_item;
public:
	X360ShaderPassDescriptor();
	virtual ~X360ShaderPassDescriptor() override;
	virtual int Unk0(char*);
	virtual int Unk1(int);
	virtual void Unk2();
	virtual int GetTextureStringTableIndex(int index);
};