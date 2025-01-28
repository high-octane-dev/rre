#include "rsstring_util.hpp"

#include <cstdio>
#include <cstdarg>
#include <cstring>

// OFFSET: 0x0055f230, STATUS: TODO
int RSStringUtil::Ssnprintf(char* buf, std::size_t len, const char* format, ...) {
	// FIXME: The original game uses a custom implementation of sprintf to avoid invoking any allocations.
	// For now, using the real deal is good enough.

	va_list args;
	va_start(args, format);
	int return_value = vsnprintf(buf, len, format, args);
	va_end(args);
	return return_value;
}

// I am so unsure about this signature...
// OFFSET: 0x0055e600, STATUS: TODO
void RSStringUtil::Svformatter(int (*format_function)(char**), char* buf, const char* format, const void* format_args) {
	// FIXME: Not yet implemented
}

// OFFSET: 0x0055f320, STATUS: TODO
const char* RSStringUtil::stristr(const char* string, const char* sub_string) {
	// not the real implementation, refer to the comment in Ssnprintf

	return strstr(string, sub_string);
}
