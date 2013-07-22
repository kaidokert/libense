#ifndef INCLUDE_DETAIL_ARRAY__WRAPPER__HPP_
#define INCLUDE_DETAIL_ARRAY__WRAPPER__HPP_

#include <detail/const_array_wrapper.hpp>

namespace ense {
namespace detail {

template<typename Target, typename Index, typename Value>
class array_wrapper : public const_array_wrapper<Target, Index, Value> {
	public:
		array_wrapper(Target* target, Index idx)
			: const_array_wrapper<Target, Index, Value>(target, idx)
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

#endif /* INCLUDE_DETAIL_ARRAY__WRAPPER__HPP_ */