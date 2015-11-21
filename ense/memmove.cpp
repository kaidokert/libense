#include <string.h>

extern "C"
void* __aeabi_memmove(void* dest, const void* src, size_t n)
{
	uintptr_t dup = reinterpret_cast<uintptr_t>(dest);
	uintptr_t sup = reinterpret_cast<uintptr_t>(src);

	if (dup < sup || dup >= sup + n) {
		return memcpy(dest, src, n);
	}

	unsigned char* d = static_cast<unsigned char*>(dest) + n - 1;
	const unsigned char* s = static_cast<const unsigned char*>(src) + n - 1;

#ifndef CONFIG_LIBC_MEMfn_BYTEWISE
	if (n >= 4) {
		while (reinterpret_cast<uintptr_t>(d) % 4) {
			*d-- = *s--;
			n--;
		}

		typedef uint32_t u32 [[gnu::may_alias]];

		if (reinterpret_cast<uintptr_t>(s) % 4 == 0) {
			u32* dl = reinterpret_cast<u32*>(d);
			const u32* sl = reinterpret_cast<const u32*>(s);

			while (n >= 4) {
				*dl-- = *sl--;
				n -= 4;
			}

			d = reinterpret_cast<unsigned char*>(dl);
			s = reinterpret_cast<const unsigned char*>(sl);
		}
	}
#endif

	while (n--) {
		*d-- = *s--;
	}

	return dest;
}
