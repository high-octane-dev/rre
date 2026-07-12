#pragma once
#include "game_object.hpp"
#include "math/matrix4x4.hpp"
#include "math/vector4.hpp"
#include "util/macros.hpp"
#include "searchable_object.hpp"

struct Portal : public GameObject {};

struct Neighbor {
	Neighbor* next;
	class SearchNode* backing_node;
	Portal* portal;
};

struct SearchMethod : public BaseObject {
	virtual ~SearchMethod() override;
	virtual void Reset();
	virtual void CalculatePointPlacementCode(float, float, float, unsigned int) = 0;
	virtual void CalculatePointPlacementCode(Vector4 const*, unsigned int) = 0;
	virtual void CalculateBoxPlacementCode(Vector4 const*, Vector4 const*, unsigned int) = 0;
	virtual void InsertObject(SearchableObject*, unsigned int, Vector4 const*, Vector4 const*) = 0;
	virtual void UpdateExtents(SearchableObject*, unsigned int, Vector4 const*, Vector4 const*);

	virtual void RemoveObject(SearchableObject*, unsigned int) = 0;
	virtual void OpenSearch(Vector4 const*, Vector4 const*, Vector4*, Vector4*) = 0;
	virtual void NextSearchObject(unsigned int) = 0;
	virtual void RewindSearch() = 0;
	virtual void CloseSearch() = 0;
	virtual void PerformPrepareGeometry() = 0;
	virtual void PerformRender3D(int) = 0;
	virtual void Initialize() = 0;
	virtual void ReleaseRootNode() = 0;
};

class SearchNode : public GameObject {
private:
	Matrix4x4 unk_matrix;
	Neighbor* neighbors;
	SearchMethod* static_search_method;
	SearchMethod* dynamic_search_method;
	void** visibility_exclusion_list;
	std::size_t visibility_exclusion_list_len;
	void** super_search_node_list;
	std::size_t super_search_node_list_len;
	int index;
	Vector4 unk_vector;
	short visibility_time_stamp;
	short visited_timestamp;
	unsigned char search_node_flags;
public:
	SearchNode(bool start_enabled);
	virtual ~SearchNode() override;
	virtual int PrepareGeometry() override;
	virtual int Render3D(int user_data) override;
};

#ifdef _M_IX86
static_assert(sizeof(SearchNode) == 0xB0);
#endif