#include <cstdio>
#include <cstdlib>
#include <exception>

void panic_impl(const char* message) {
	printf(message);
#ifdef _DEBUG
	throw std::exception();
#else
	std::abort();
#endif // DEBUG

}