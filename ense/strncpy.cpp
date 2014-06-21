#include <string.h>

extern "C"
char* strncpy(char* dest, const char* src, size_t n)
{
	char* target = dest;

	if (n) {
		do {
			*dest++ = *src;
			n--;
		} while (n && *src++);

		memset(dest, 0, n);
	}

	return target;
}
