#include <string.h>

const char* strchr(const char* s, int c)
{
	while (*s && *s != c)
		s++;

	return *s == c ? s : nullptr;
}
