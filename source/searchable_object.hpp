#pragma once

class SearchableObject {
public:
	unsigned short search_time_stamp;
	unsigned char unk_type;
	unsigned char searchable_object_flags;

	virtual ~SearchableObject();
	inline void SetSearchTimeStamp(unsigned short time_stamp) {
		search_time_stamp = time_stamp;
	}
	inline unsigned short GetSearchTimeStamp() {
		return search_time_stamp;
	}
};