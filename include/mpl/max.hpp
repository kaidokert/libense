#ifndef INCLUDE_MPL_MAX__HPP_
#define INCLUDE_MPL_MAX__HPP_

namespace ense {
namespace mpl {

constexpr uint32_t max(uint32_t arg)
{
	return arg;
}

constexpr uint32_t max(uint32_t a, uint32_t b)
{
	return a > b ? a : b;
}

template<typename... Args>
constexpr uint32_t max(uint32_t arg1, uint32_t arg2, Args... args)
{
	return max(max(arg1, arg2), max(args...));
}

}
}

#endif /* INCLUDE_MPL_MAX__HPP_ */
