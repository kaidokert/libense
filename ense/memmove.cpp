#include <string.h>
#include <stdint.h>

[[gnu::alias("memmove")]]
extern "C" void __aeabi_memmove(void*, const void*, size_t);

void* memmove(void* dest, const void* src, size_t n)
{
	uintptr_t dup = reinterpret_cast<uintptr_t>(dest);
	uintptr_t sup = reinterpret_cast<uintptr_t>(src);

	if (dup < sup || dup >= sup + n) {
		return memcpy(dest, src, n);
	}

	size_t dOverhang = dup % sizeof(uint32_t);
	size_t sOverhang = sup % sizeof(uint32_t);

	void* target = dest;

	char* d = reinterpret_cast<char*>(dest) + n;
	const char* s = reinterpret_cast<const char*>(src) + n;

	while (dOverhang == sOverhang && (dup + n) % sizeof(uint32_t)) {
		*--d = *--s;
		n--;
	}

	if (!dOverhang && !sOverhang) {
		uint32_t* dl = reinterpret_cast<uint32_t*>(d);
		const uint32_t* sl = reinterpret_cast<const uint32_t*>(s);

		while (n >= sizeof(uint32_t)) {
			*--dl = *--sl;
			n -= sizeof(uint32_t);
		}
	}

	while (n-- > 0) {
		*--d = *--s;
	}

	return target;
}
