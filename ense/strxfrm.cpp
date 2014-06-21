#include <string.h>

extern "C"
size_t strxfrm(char* dest, const char* src, size_t n)
{
	size_t len = strlen(src);

	if (n < len + 1)
		return len;

	memcpy(dest, src, len + 1);
	return len;
}
