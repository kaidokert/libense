#ifndef INCLUDE_MPL_NTH__ARG__HPP_3B681A8BBF405B7A
#define INCLUDE_MPL_NTH__ARG__HPP_3B681A8BBF405B7A

namespace ense {
namespace mpl {

template<size_t N, typename Fn>
struct nth_arg;

template<size_t N, typename R, typename C, typename... Args>
struct nth_arg<N, R (C::*)(Args...)> : nth_arg<N, R(Args...)> {
};

template<size_t N, typename R, typename FirstArg, typename... Rest>
struct nth_arg<N, R(FirstArg, Rest...)> : nth_arg<N - 1, R(Rest...)> {
};

template<typename R, typename FirstArg, typename... Rest>
struct nth_arg<0, R(FirstArg, Rest...)> {
	typedef FirstArg type;
};

}
}

#endif /* INCLUDE_MPL_NTH__ARG__HPP_3B681A8BBF405B7A */
