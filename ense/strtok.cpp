#include <string.h>

extern "C"
char* strtok(char* s1, const char* s2)
{
	static char* last = nullptr;

	if (!s1)
		s1 = last;
	if (!s1)
		return nullptr;

	size_t offset = strspn(s1, s2);

	if (!s1[offset])
		return nullptr;

	s1 += offset;

	size_t len = strcspn(s1, s2);

	if (s1[len]) {
		s1[len] = 0;
		last = s1 + len + 1;
	} else {
		last = nullptr;
	}

	return s1;
}
