#include <string.h>

const void* memchr(const void* m, int c, size_t n)
{
	const unsigned char* pos = static_cast<const unsigned char*>(m);
	unsigned char val = c;

	while (n && *pos != val) {
		pos++;
		n--;
	}

	return n ? pos : nullptr;
}
