#pragma once
#include "game_object.hpp"
#include "math/matrix4x4.hpp"
#include "dynamic_searchable_object.hpp"
#include "util/macros.hpp"

class Camera : public DynamicSearchableObject, GameObject {
protected:
    SearchNode* search_node;
    Matrix4x4 projection_matrix;
    Matrix4x4 view_matrix;
    Matrix4x4 view_projection_matrix;
    Matrix4x4 identity;
public:
    Vector4 position;
private:
    Vector4 look_vector;
    Vector4 up_vector;
    float fov_degrees;
    float aspect_ratio;
    float far_plane_distance;
    float unk_aspect_scale;
    D3DVIEWPORT9 viewport;
    float viewport_to_camera_aspect;
    float near_clip_distance;
    float far_clip_distance;
public:
    int max_viewport_width;
    int max_viewport_height;
private:
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
    std::uint8_t camera_flags;
public:
    int enable_perspective;
public:
    Camera(bool start_enabled);
    virtual ~Camera() override;
    virtual void Disable() override;
    virtual void Enable() override;
    virtual GameObject* Create() override;
    virtual int Tick(float) override;
    virtual int Restore() override;
    virtual int KeyDownHandler(ProcessNode*, KeyInfo*) override;

    virtual GameObject* Create(float, int, int);
    virtual void DumpSnapShot();
    virtual void SetDumpCubeViewParameters();
    virtual void DumpCubeFace();
    virtual void SetDumpSpinViewParameters();
    virtual void DumpSpin(Vector4*, Vector4*, int);
    virtual void DumpSpinFrame();
    virtual void UpdateMatrices();
    virtual void ComputeGuardBandMatrix(Matrix4x4*, Matrix4x4*, float, float);
    virtual void UpdateClipDistances(float, float);
    virtual int LookAt(Vector4*, Vector4*);
    virtual int SurfaceChanged(float, int, int);

    void AddSearchPlacement(SearchNode*, unsigned int);
    void GetViewParameters(Vector4*, Vector4*, Vector4*, float*, float*);
    void ResetSearchPlacement(SearchNode*, unsigned int);
    int ResizeViewport(int, int, int, int);
    void SetEnablePerspective(int);
    void SetFOVDegrees(float);
    void SetUpViewportAndMatrices(int, int, int, int);
    // Last Laugh not limelight (iykyk)
    int SetUpViewportForLL(int, int, int, int, float);
    void SetViewParameters(Vector4*, Vector4*, Vector4*, float*, float*);
    void SetViewport(D3DVIEWPORT9*, D3DVIEWPORT9*);
};