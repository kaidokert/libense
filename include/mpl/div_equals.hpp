#ifndef INCLUDE_MPL_DIV__EQUALS__HPP_B42BB420F39C59FA
#define INCLUDE_MPL_DIV__EQUALS__HPP_B42BB420F39C59FA

#include <type_traits>

namespace ense {
namespace mpl {

template<typename T, T Divisor, T Value>
struct div_equals {
	template<typename In>
	struct fn;

	template<T In>
	struct fn<std::integral_constant<T, In>> :
		std::integral_constant<bool, In / Divisor == Value> {
	};
};

}
}

#endif /* INCLUDE_MPL_DIV__EQUALS__HPP_B42BB420F39C59FA */
