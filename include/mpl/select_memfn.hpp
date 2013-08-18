#ifndef INCLUDE_MPL_SELECT__MEMFN__HPP_95649292AA95CFBF
#define INCLUDE_MPL_SELECT__MEMFN__HPP_95649292AA95CFBF

namespace ense {
namespace mpl {

template<typename Result, typename Class>
constexpr auto select_memfn0c(Result (Class::*fn)() const)
	-> Result (Class::*)() const
{
	return fn;
}

template<typename Result, typename Class, typename Arg1>
constexpr auto select_memfn1(Result (Class::*fn)(Arg1))
	-> Result (Class::*)(Arg1)
{
	return fn;
}

template<typename Result, typename Class, typename Arg1>
constexpr auto select_memfn1c(Result (Class::*fn)(Arg1) const)
	-> Result (Class::*)(Arg1) const
{
	return fn;
}

template<typename Result, typename Class, typename Arg1, typename Arg2>
constexpr auto select_memfn2(Result (Class::*fn)(Arg1, Arg2))
	-> Result (Class::*)(Arg1, Arg2)
{
	return fn;
}

}
}

#endif /* INCLUDE_MPL_SELECT__MEMFN__HPP_95649292AA95CFBF */
