#pragma once

class SearchableObject {
private:
	unsigned short search_time_stamp;
	unsigned char unk_type;
	unsigned char flags;
public:
	virtual ~SearchableObject();
	inline void SetSearchTimeStamp(unsigned short time_stamp) {
		search_time_stamp = time_stamp;
	}
	inline unsigned short GetSearchTimeStamp() {
		return search_time_stamp;
	}
};