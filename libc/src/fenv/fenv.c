#include <fenv.h>

int feclearexcept(int e)
{
	fenv_t env;
	fegetenv(&env);
	env.__cw &= ~(e & FE_ALL_EXCEPT);
	return fesetenv(&env);
}

int feraiseexcept(int e)
{
	fenv_t env;
	fegetenv(&env);
	env.__cw |= e & FE_ALL_EXCEPT;
	return fesetenv(&env);
}

int fetestexcept(int e)
{
	fenv_t env;
	fegetenv(&env);
	return env.__cw & e & FE_ALL_EXCEPT;
}

int fegetround(void)
{
	fenv_t env;
	fegetenv(&env);
	return env.__cw & (FE_TONEAREST | FE_DOWNWARD | FE_UPWARD | FE_TOWARDZERO);
}

int fesetround(int r)
{
	fenv_t env;
	fegetenv(&env);
	env.__cw &= ~(FE_TONEAREST | FE_DOWNWARD | FE_UPWARD | FE_TOWARDZERO);
	env.__cw |= r & (FE_TONEAREST | FE_DOWNWARD | FE_UPWARD | FE_TOWARDZERO);
	return fesetenv(&env);
}
