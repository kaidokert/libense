#include <string.h>

extern "C"
char* strcpy(char* dest, const char* src)
{
	char* target = dest;

	while ((*dest++ = *src++));

	return target;
}
