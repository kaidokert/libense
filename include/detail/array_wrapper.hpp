#ifndef INCLUDE_DETAIL_ARRAY__WRAPPER__HPP_7436E64E2CA426E3
#define INCLUDE_DETAIL_ARRAY__WRAPPER__HPP_7436E64E2CA426E3

#include <detail/const_array_wrapper.hpp>

namespace ense {
namespace detail {

template<typename Target, typename Value>
class array_wrapper : public const_array_wrapper<Target, Value> {
	public:
		array_wrapper(Target* target, uint32_t idx)
			: const_array_wrapper<Target, Value>(target, idx)
		{
		}

		Value operator=(Value val)
		{
			this->target->set(this->idx, val);
			return val;
		}

		operator Value&()
		{
			return this->target->get(this->idx);
		}
};

}
}

#endif /* INCLUDE_DETAIL_ARRAY__WRAPPER__HPP_7436E64E2CA426E3 */
