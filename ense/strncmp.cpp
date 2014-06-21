#include <string.h>

extern "C"
int strncmp(const char* s1, const char* s2, size_t n)
{
	while (n && *s1 && *s1 == *s2) {
		s1++;
		s2++;
		n--;
	}

	return n ? *s1 - *s2 : 0;
}
