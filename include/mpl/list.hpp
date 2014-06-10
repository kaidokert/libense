#ifndef INCLUDE_MPL_LIST__HPP_092B77D39F5C1127
#define INCLUDE_MPL_LIST__HPP_092B77D39F5C1127

#include <type_traits>

namespace ense {
namespace mpl {


template<typename... Items>
struct list {
	typedef list type;
};



namespace detail {

	template<typename T, T First, T Last, T... List>
	struct make_range : make_range<T, First + 1, Last, List..., First> {
	};

	template<typename T, T Only, T... List>
	struct make_range<T, Only, Only, List...> {
		typedef list<std::integral_constant<T, List>..., std::integral_constant<T, Only>> type;
	};

}

template<typename T, T Begin, T End>
using make_range = typename detail::make_range<T, Begin, End>::type;

}
}

#endif /* INCLUDE_MPL_LIST__HPP_092B77D39F5C1127 */
