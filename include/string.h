#ifndef STRING_JBZT8EJJ
#define STRING_JBZT8EJJ

#include <stddef.h>
#include <stdint.h>

extern "C" {

[[gnu::always_inline]]
inline void* memcpy(void* dest, const void* src, size_t n)
{
	return __builtin_memcpy(dest, src, n);
}

[[gnu::always_inline]]
inline void* memmove(void* dest, const void* src, size_t n)
{
	return __builtin_memmove(dest, src, n);
}

[[gnu::always_inline]]
inline void* memset(void* dest, int c, size_t n)
{
	return __builtin_memset(dest, c, n);
}

char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);
int memcmp(const void* m1, const void* m2, size_t n);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);

[[gnu::always_inline]]
inline int strcoll(const char* s1, const char* s2)
{
	return strcmp(s1, s2);
}

size_t strxfrm(char* dest, const char* src, size_t n);

}

const void* memchr(const void* m, int c, size_t n);

[[gnu::always_inline]]
inline void* memchr(void* m, int c, size_t n)
{
	return const_cast<void*>(memchr(const_cast<const void*>(m), c, n));
}

const char* strchr(const char* m, int c);

[[gnu::always_inline]]
inline char* strchr(char* s, int c)
{
	return const_cast<char*>(strchr(const_cast<const char*>(s), c));
}

extern "C" size_t strcspn(const char* s1, const char* s2);

inline const char* strpbrk(const char* s1, const char* s2)
{
	const char* s = s1 + strcspn(s1, s2);

	return *s ? nullptr : s;
}

[[gnu::always_inline]]
inline char* strpbrk(char* s1, const char* s2)
{
	return const_cast<char*>(strpbrk(const_cast<const char*>(s1), s2));
}

const char* strrchr(const char* s, int c);

[[gnu::always_inline]]
inline char* strrchar(char* s, int c)
{
	return const_cast<char*>(strrchr(const_cast<const char*>(s), c));
}

extern "C" size_t strspn(const char* s1, const char* s2);

const char* strstr(const char* s1, const char* s2);

[[gnu::always_inline]]
inline char* strstr(char* s1, const char* s2)
{
	return const_cast<char*>(strstr(const_cast<const char*>(s1), s2));
}

extern "C" {

char* strtok(char* s1, const char* s2);

inline char* strerror(int)
{
	return nullptr;
}

size_t strlen(const char* s);

}

#endif
