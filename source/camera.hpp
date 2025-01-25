#pragma once
#include "game_object.hpp"
#include "math/matrix4x4.hpp"
#include "dynamic_searchable_object.hpp"

class Camera : public DynamicSearchableObject, GameObject {
protected:
    SearchNode* search_node;
    int unused0[3];
    Matrix4x4 projection_matrix;
    Matrix4x4 view_matrix;
    Matrix4x4 view_projection_matrix;
    Matrix4x4 identity;
    Vector4 position;
    Vector4 look_vector;
    Vector4 up_vector;
    float fov_degrees;
    float aspect_ratio;
    float near_plane_distance;
    float unk_aspect_scale;
    D3DVIEWPORT9 viewport;
    float viewport_to_camera_aspect;
    float far_clip_distance;
    float near_clip_distance;
    int max_viewport_width;
    int max_viewport_height;
    int unk_timer;
    int unk_render_target_related;
    float unused1;
    float unused2;
    int is_last_laugh;
    float last_laugh_shift;
    Vector4 position_copy;
    Vector4 look_vector_copy;
    Vector4 up_vector_copy;
    float up_vector_norm;
    float asin_look_angle;
    Vector4 unused3;
    std::int8_t dump_cube_tick;
    std::int8_t dump_spin_tick;
    Vector4 dump_spin_position;
    Vector4 dump_spin_look;
    Vector4 dump_spin_up;
    Vector4 dump_spin_axis;
    Vector4 dump_spin_offset;
    int dump_spin_cycle_count;
    Vector4* dump_cube_position;
    std::uint8_t flags;
    int enable_perspective;
public:
};