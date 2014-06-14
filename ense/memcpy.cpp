#include <string.h>
#include <stdint.h>

[[gnu::alias("memcpy")]]
extern "C" void __aeabi_memcpy(void*, const void*, size_t);

void* memcpy(void* dest, const void* src, size_t n)
{
	size_t dOverhang = reinterpret_cast<uintptr_t>(dest) % sizeof(uint32_t);
	size_t sOverhang = reinterpret_cast<uintptr_t>(src) % sizeof(uint32_t);

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
