#include <string.h>

extern "C"
size_t strspn(const char* s1, const char* s2)
{
	size_t len = 0;

	while (*s1) {
		const char* t = s2;

		while (*t && *s1 != *t)
			t++;

		if (!*t)
			break;

		len++;
		s1++;
	}

	return len;
}
