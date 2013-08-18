#ifndef INCLUDE_MPL_RESULT__TYPE__HPP_EE431323000EB363
#define INCLUDE_MPL_RESULT__TYPE__HPP_EE431323000EB363

namespace ense {
namespace mpl {

template<typename>
struct result_type;

template<typename R, typename C, typename... Args>
struct result_type<R (C::*)(Args...) const> : result_type<R (C::*)(Args...)> {
};

template<typename R, typename C, typename... Args>
struct result_type<R (C::*)(Args...)> {
	typedef R type;
};

}
}

#endif /* INCLUDE_MPL_RESULT__TYPE__HPP_EE431323000EB363 */
