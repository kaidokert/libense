#ifndef INCLUDE_BITMASK__HPP_0165DF979A4D958C
#define INCLUDE_BITMASK__HPP_0165DF979A4D958C

#include <type_traits>

namespace ense {

namespace detail {
namespace bitmask {

template<typename Bit>
constexpr typename std::underlying_type<Bit>::type bitmask(Bit bit)
{
	return static_cast<typename std::underlying_type<Bit>::type>(bit);
}

template<typename Bit, typename... Rest>
constexpr typename std::underlying_type<Bit>::type bitmask(Bit bit, Rest... rest)
{
	return bitmask(rest...) | bitmask(bit);
}

}
}

template<typename... Args>
constexpr auto bitmask(Args... args)
	-> typename std::underlying_type<typename std::common_type<Args...>::type>::type
{
	return detail::bitmask::bitmask(args...);
}

}

#endif /* INCLUDE_BITMASK__HPP_0165DF979A4D958C */
