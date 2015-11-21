#include <string.h>

extern "C"
void* __aeabi_memset(void* dest, size_t n, int c)
{
	unsigned char fill = c;
	unsigned char* d = static_cast<unsigned char*>(dest);

#ifndef CONFIG_LIBC_MEMfn_BYTEWISE
	if (n >= 4) {
		while (reinterpret_cast<uintptr_t>(d) % 4) {
			*d++ = fill;
			n--;
		}

		typedef uint32_t u32 [[gnu::may_alias]];

		u32* dl = reinterpret_cast<u32*>(d);
		uint16_t fillh = (fill << 8) | fill;
		uint32_t fillw = (fillh << 16) | fillh;

		while (n >= 4) {
			*dl++ = fillw;
			n -= 4;
		}

		d = reinterpret_cast<unsigned char*>(dl);
	}
#endif

	while (n-- > 0) {
		*d++ = fill;
	}

	return dest;
}
