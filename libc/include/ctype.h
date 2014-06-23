#ifndef	_CTYPE_H
#define	_CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

inline int isalpha(int c) { return ((unsigned)c|32)-'a' < 26; }
inline int isblank(int c) { return (c == ' ' || c == '\t'); }
inline int iscntrl(int c) { return (unsigned)c < 0x20 || c == 0x7f; }
inline int isdigit(int c) { return (unsigned)c-'0' < 10; }
inline int isgraph(int c) { return (unsigned)c-0x21 < 0x5e; }
inline int islower(int c) { return (unsigned)c-'a' < 26; }
inline int isprint(int c) { return (unsigned)c-0x20 < 0x5f; }
inline int isspace(int c) { return c == ' ' || (unsigned)c-'\t' < 5; }
inline int isupper(int c) { return (unsigned)c-'A' < 26; }

inline int isalnum(int c) { return isalpha(c) || isdigit(c); }
inline int ispunct(int c) { return isgraph(c) && !isalnum(c); }
inline int isxdigit(int c) { return isdigit(c) || ((unsigned)c|32)-'a' < 6; }
inline int tolower(int c) { return isupper(c) ? c | 32 : c; }
inline int toupper(int c) { return islower(c) ? c & 0x5f : c; }

#ifdef __cplusplus
}
#endif

#endif
