#ifndef INCLUDE_MPL_ALL__HPP_A112E6586552A48B
#define INCLUDE_MPL_ALL__HPP_A112E6586552A48B

namespace ense {
namespace mpl {

constexpr bool all()
{
	return true;
}

template<typename... Rest>
constexpr bool all(bool arg1, Rest... rest)
{
	return arg1 && all(rest...);
}

}
}

#endif /* INCLUDE_MPL_ALL__HPP_A112E6586552A48B */
