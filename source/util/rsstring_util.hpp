#pragma once

#include <cstddef>

namespace RSStringUtil {
	int Ssnprintf(char* buf, std::size_t len, const char* format, ...);
	void Svformatter(int (*format_function)(char**), char* buf, const char* format, const void* format_args);
	const char* stristr(const char* string, const char* sub_string);
	void ChangeExtension(char* path, char* string);
}
