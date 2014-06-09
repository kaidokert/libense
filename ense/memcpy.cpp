#include <string.h>
#include <stdint.h>

void* memcpy(void* dest, const void* src, size_t n)
{
	size_t dOverhang = reinterpret_cast<intptr_t>(dest) & (sizeof(uint32_t) - 1);
	size_t sOverhang = reinterpret_cast<intptr_t>(src) & (sizeof(uint32_t) - 1);

	void* target = dest;

	if (!dOverhang && !sOverhang) {
		uint32_t* d = reinterpret_cast<uint32_t*>(dest);
		const uint32_t* s = reinterpret_cast<const uint32_t*>(src);

		while (n >= sizeof(uint32_t)) {
			*d++ = *s++;
			n -= sizeof(uint32_t);
		}

		dest = d;
		src = s;
	}

	char* d = reinterpret_cast<char*>(dest);
	const char* s = reinterpret_cast<const char*>(src);

	while (n-- > 0) {
		*d++ = *s++;
	}

	return target;
}
