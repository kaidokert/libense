#include <string.h>

extern "C"
int memcmp(const void* m1, const void* m2, size_t n)
{
	const unsigned char* l = static_cast<const unsigned char*>(m1);
	const unsigned char* r = static_cast<const unsigned char*>(m2);

#ifndef CONFIG_LIBC_MEMfn_BYTEWISE
	if (n >= 4) {
		while (reinterpret_cast<uintptr_t>(l) % 4) {
			int diff = *l++ - *r++;

			if (diff)
				return diff;

			n--;
		}

		typedef uint32_t u32 [[gnu::may_alias]];

		if (reinterpret_cast<uintptr_t>(r) % 4 == 0) {
			const u32* ll = reinterpret_cast<const u32*>(l);
			const u32* rl = reinterpret_cast<const u32*>(r);

			while (n >= 4) {
				if (*ll != *rl)
					break;

				n -= 4;
				ll++;
				rl++;
			}

			l = reinterpret_cast<const unsigned char*>(ll);
			r = reinterpret_cast<const unsigned char*>(rl);
		}
	}
#endif

	while (n--) {
		int diff = *l++ - *r++;

		if (diff)
			return diff;
	}

	return 0;
}
