#pragma once

namespace RSStringUtil {
	int Ssnprintf(char* buf, int len, const char* format, ...);
	void Svformatter(int (*format_function)(), char* buf, const char* format, const void* format_args);
}
