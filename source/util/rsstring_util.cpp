#include "rsstring_util.hpp"

#include <cstdio>
#include <cstdarg>
#include <cstring>

// OFFSET: 0x0055f230
int RSStringUtil::Ssnprintf(char* buf, std::size_t len, const char* format, ...) {
	// ft said something about if it walks like a duck its a duck or some shit.
	// I really don't want to reverse the Svformatter function.

	// Clang gets annoyed here because of the format variable (this can potential enable exploits)
	// there is probably no way to make it happy.

	va_list args;
	va_start(args, format);
	int return_value = vsnprintf(buf, len, format, args);
	va_end(args);
	return return_value;
}

// I am so unsure about this signature...
// OFFSET: 0x0055e600
void RSStringUtil::Svformatter(int (*format_function)(char**), char* buf, const char* format, const void* format_args) {
	// TODO: Not yet implemented
}

// OFFSET: 0x0055f320
const char* RSStringUtil::stristr(const char* string, const char* sub_string) {
	// not the real implementation, refer to the comment in Ssnprintf

	return strstr(string, sub_string);
}
