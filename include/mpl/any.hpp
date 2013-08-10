#ifndef INCLUDE_MPL_ANY__HPP_45A3AFBF48458F4B
#define INCLUDE_MPL_ANY__HPP_45A3AFBF48458F4B

namespace ense {
namespace mpl {

constexpr bool any()
{
	return false;
}

template<typename... Args>
constexpr bool any(bool arg1, Args... args)
{
	return arg1 || any(args...);
}

}
}

#endif /* INCLUDE_MPL_ANY__HPP_45A3AFBF48458F4B */
