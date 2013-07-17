#ifndef INCLUDE_MPL_CAST__HPP_
#define INCLUDE_MPL_CAST__HPP_

#include <type_traits>

namespace ense {
namespace mpl {

template<typename To>
struct static_cast_to {
	template<typename Value>
	struct apply {
		typedef std::integral_constant<To, static_cast<To>(Value::value)> type;
	};
};

}
}

#endif /* INCLUDE_MPL_CAST__HPP_ */
