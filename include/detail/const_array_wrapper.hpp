#ifndef INCLUDE_DETAIL_CONST__ARRAY__WRAPPER__HPP_
#define INCLUDE_DETAIL_CONST__ARRAY__WRAPPER__HPP_

namespace ense {
namespace detail {

template<typename Target, typename Index, typename Value>
class const_array_wrapper {
	protected:
		Target* target;
		Index idx;

	public:
		const_array_wrapper(Target* target, Index idx)
			: target(target), idx(idx)
		{
		}

		operator const Value&() const
		{
			return target->get(idx);
		}

		template<typename T>
		operator T() const
		{
			return static_cast<T>(target->get(idx));
		}
};

}
}

#endif /* INCLUDE_DETAIL_CONST__ARRAY__WRAPPER__HPP_ */
