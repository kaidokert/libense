#include <string.h>
#include <stdint.h>

void* memset(void* dest, int c, size_t n)
{
	for (char* d = reinterpret_cast<char*>(dest); n > 0; n--) {
		*d++ = static_cast<char>(c);
	}

	return dest;
}
