#include <string.h>

const char* strrchr(const char* s, int c)
{
	size_t len = strlen(s);

	while (len > 0 && s[len] != c)
		len--;

	if (len)
		return s + len;
	else
		return *s == c ? s : nullptr;
}
