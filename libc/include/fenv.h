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

inline int feclearexcept(int e)
{
	fenv_t env;
	fegetenv(&env);
	env.__cw &= ~(e & FE_ALL_EXCEPT);
	return fesetenv(&env);
}

inline int feraiseexcept(int e)
{
	fenv_t env;
	fegetenv(&env);
	env.__cw |= e & FE_ALL_EXCEPT;
	return fesetenv(&env);
}

inline int fetestexcept(int e)
{
	fenv_t env;
	fegetenv(&env);
	return env.__cw & e & FE_ALL_EXCEPT;
}

inline int fegetround(void)
{
	fenv_t env;
	fegetenv(&env);
	return env.__cw & (FE_TONEAREST | FE_DOWNWARD | FE_UPWARD | FE_TOWARDZERO);
}

inline int fesetround(int r)
{
	fenv_t env;
	fegetenv(&env);
	env.__cw &= ~(FE_TONEAREST | FE_DOWNWARD | FE_UPWARD | FE_TOWARDZERO);
	env.__cw |= r & (FE_TONEAREST | FE_DOWNWARD | FE_UPWARD | FE_TOWARDZERO);
	return fesetenv(&env);
}

int fegetexceptflag(fexcept_t *, int);
int fesetexceptflag(const fexcept_t *, int);
int feholdexcept(fenv_t *);
int feupdateenv(const fenv_t *);

#ifdef __cplusplus
}
#endif
#endif

