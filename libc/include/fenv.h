#ifndef _FENV_H
#define _FENV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bits/fenv.h>

inline int fegetenv(fenv_t *env)
{
	__asm__ __volatile__ ("vmrs %0, FPSCR" : "=r"(env->__cw));
	return 0;
}

inline int fesetenv(const fenv_t *env)
{
	unsigned long v = env != FE_DFL_ENV ? env->__cw : 0;
	__asm__ __volatile__ ("vmsr FPSCR, %0" : : "r"(v));
	return 0;
}

int feclearexcept(int);
int fegetexceptflag(fexcept_t *, int);
int feraiseexcept(int);
int fesetexceptflag(const fexcept_t *, int);
int fetestexcept(int);

int fegetround(void);
int fesetround(int);

int feholdexcept(fenv_t *);
int feupdateenv(const fenv_t *);

#ifdef __cplusplus
}
#endif
#endif

