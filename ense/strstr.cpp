#include <string.h>

const char* strstr(const char* s1, const char* s2)
{
	if (!*s2)
		return s1;

	while (*s1) {
		const char* match = s2;

		while (*s1 && *match && *s1 == *match) {
			s1++;
			match++;
		}

		if (!*match)
			return s1;
	}

	return nullptr;
}
