#ifndef INCLUDE_MPL_MIN__HPP_BF0FDC41C433CF58
#define INCLUDE_MPL_MIN__HPP_BF0FDC41C433CF58

namespace ense {
namespace mpl {

constexpr uint32_t min(uint32_t arg)
{
	return arg;
}

constexpr uint32_t min(uint32_t a, uint32_t b)
{
	return a < b ? a : b;
}

template<typename... Args>
constexpr uint32_t min(uint32_t arg1, uint32_t arg2, Args... args)
{
	return min(min(arg1, arg2), min(args...));
}

}
}

#endif /* INCLUDE_MPL_MIN__HPP_BF0FDC41C433CF58 */
