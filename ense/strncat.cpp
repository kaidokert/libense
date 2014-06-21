#include <string.h>

extern "C"
char* strncat(char* dest, const char* src, size_t n)
{
	char* target = dest;

	if (n) {
		dest += strlen(dest);

		while (n && *src) {
			*dest++ = *src++;
			n--;
		}
		*dest = 0;
	}

	return target;
}
