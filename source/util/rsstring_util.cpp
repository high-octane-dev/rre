#include "rsstring_util.hpp"

#include <cstdio>

// OFFSET: 0x0055f230
int RSStringUtil::Ssnprintf(char* buf, std::size_t len, const char* format, ...) {
	// ft said something about if it walks like a duck its a duck or some shit.
	// I really don't want to reverse the Svformatter function.

	return snprintf(buf, len, format);
}

// I am so unsure about this signature...
// OFFSET: 0x0055e600
void RSStringUtil::Svformatter(int (*format_function)(char**), char* buf, const char* format, const void* format_args) {
	// TODO: Not yet implemented
}
