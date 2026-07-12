#include "search_node.hpp"

// OFFSET: 0x0055fd20, STATUS: TODO
SearchNode::SearchNode(bool start_enabled) : GameObject(start_enabled) {
	search_node_flags &= 0xFE;
	unk_vector.x = -1.0;
	unk_vector.y = 1.0;
	unk_vector.z = -1.0;
	unk_vector.w = 1.0;
	neighbors = nullptr;
	static_search_method = nullptr;
	dynamic_search_method = nullptr;
	index = 0;
	visibility_exclusion_list = nullptr;
	visibility_exclusion_list_len = 0;
	super_search_node_list = nullptr;
	super_search_node_list_len = 0;
	// _DAT_00714924 = 0;
	// g_WholeScreen[0][1] = -1.0;
	// g_WholeScreen[0][0] = -1.0;
	// g_WholeScreen[1][0] = 1.0;
	// g_WholeScreen[0][2] = -1.0;
	// g_WholeScreen[0][3] = 1.0;
	// g_WholeScreen[1][3] = -1.0;
	// g_WholeScreen[1][1] = 1.0;
	// g_WholeScreen[1][2] = 1.0;
	// FUN_0055b0c0(g_WholeScreen);
}

// OFFSET: 0x0055fe50, STATUS: COMPLETE
SearchNode::~SearchNode() {
	Neighbor* current = neighbors;
	while (current != nullptr) {
		Neighbor* next = current->next;
		delete current;
		current = next;
	}
	if (visibility_exclusion_list != 0) {
		delete[] visibility_exclusion_list;
	}
	if (super_search_node_list != 0) {
		delete[] super_search_node_list;
	}
	if (static_search_method != nullptr) {
		static_search_method->Release();
		static_search_method = nullptr;
	}
	if (dynamic_search_method != nullptr) {
		dynamic_search_method->Release();
		dynamic_search_method = nullptr;
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
int SearchNode::PrepareGeometry() {
	return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
int SearchNode::Render3D(int user_data) {
	return 0;
}
