#include <string.h>
#include <stdint.h>

static void memcpy8(uint8_t* dest, const uint8_t* src, size_t n)
{
	while (n > 0) {
		*dest++ = *src++;
		n--;
	}
}

static void memcpy32(uint32_t* dest, const uint32_t* src, size_t n)
{
	while (n > 4) {
		*dest++ = *src++;
		n -= 4;
	}
	if (n > 0) {
		memcpy8(reinterpret_cast<uint8_t*>(dest),
				reinterpret_cast<const uint8_t*>(src),
				n);
	}
}

void* memcpy(void* dest, const void* src, size_t n)
{
	size_t dOverhang = reinterpret_cast<intptr_t>(dest) & (sizeof(uint32_t) - 1);
	size_t sOverhang = reinterpret_cast<intptr_t>(src) & (sizeof(uint32_t) - 1);

	if (dOverhang || sOverhang) {
		if (dOverhang != sOverhang) {
			memcpy8(reinterpret_cast<uint8_t*>(dest),
					reinterpret_cast<const uint8_t*>(src),
					n);
		} else {
			uint8_t* d8 = reinterpret_cast<uint8_t*>(dest);
			const uint8_t* s8 = reinterpret_cast<const uint8_t*>(src);

			memcpy8(d8, s8, dOverhang);
			memcpy32(reinterpret_cast<uint32_t*>(d8 + dOverhang),
					reinterpret_cast<const uint32_t*>(s8 + dOverhang),
					n - dOverhang);
		}
	} else {
		memcpy32(reinterpret_cast<uint32_t*>(dest),
				reinterpret_cast<const uint32_t*>(src),
				n);
	}

	return dest;
}
