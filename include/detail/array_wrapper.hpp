#ifndef INCLUDE_DETAIL_ARRAY__WRAPPER__HPP_7436E64E2CA426E3
#define INCLUDE_DETAIL_ARRAY__WRAPPER__HPP_7436E64E2CA426E3

#include <type_traits>

namespace ense {
namespace detail {

template<typename Target, typename Value>
class array_wrapper {
	private:
		Target* target;
		uint32_t idx;

	public:
		array_wrapper(Target* target, uint32_t idx)
			: target(target), idx(idx)
		{
		}

		array_wrapper& operator=(Value val)
		{
			target->set(idx, val);
			return *this;
		}

		operator Value() const
		{
			return target->get(idx);
		}

		void* operator&() = delete;

		template<typename T, typename = typename std::enable_if<std::is_convertible<Value, T>::value>::type>
		operator T() const
		{
			return target->get(idx);
		}
};

}
}

#endif /* INCLUDE_DETAIL_ARRAY__WRAPPER__HPP_7436E64E2CA426E3 */
