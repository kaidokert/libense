#ifndef INCLUDE_BIT__HPP_
#define INCLUDE_BIT__HPP_

namespace ense {
namespace bit {

template<size_t Bit>
struct begin : std::integral_constant<size_t, Bit> {
	typedef begin begin_t;
};

template<size_t Bit>
struct end : std::integral_constant<size_t, Bit> {
	typedef end end_t;
};

template<size_t Width>
struct width : std::integral_constant<size_t, Width> {
	typedef width width_t;
};

template<size_t Bound1, size_t Bound2>
struct range :
	std::conditional<Bound1 < Bound2, begin<Bound1>, begin<Bound2>>::type,
	std::conditional<Bound1 < Bound2, end<Bound2>, end<Bound1>>::type {};

namespace detail {

	struct has_index_tag {};

}

template<typename Idx>
struct index : detail::has_index_tag {
	typedef Idx index_t;
};

namespace detail {
	template<typename... Args>
	struct expand : expand<Args>... {};

	template<typename Mismatch>
	struct expand<Mismatch>;

	template<size_t Bound1, size_t Bound2>
	struct expand<range<Bound1, Bound2>> : range<Bound1, Bound2> {
		static constexpr uint32_t begin = expand::begin_t::value;
		static constexpr uint32_t end = expand::end_t::value;

		static_assert(end < 32, "Upper >= 32");
		static_assert(begin < end, "Lower >= Upper");

		static constexpr uint32_t range = end - begin + 1;
		static constexpr uint32_t field_anchored_mask = 0xFFFFFFFF >> (32 - range);
		static constexpr uint32_t field_mask = field_anchored_mask << begin;
	};

	template<size_t Width>
	struct expand<width<Width>> {
		static_assert(Width < 32, "Width >= 32");

		static constexpr uint32_t width = Width;
		static constexpr uint32_t array_anchored_mask = (1U << width) - 1;
	};

	template<typename Idx>
	struct expand<index<Idx>> : index<Idx> {};



	template<typename... Args>
	struct index_type : index<uint32_t> {};

	template<typename First, typename... Rest>
	struct index_type<First, Rest...> : index_type<Rest...> {};
	
	template<typename Idx, typename... Rest>
	struct index_type<index<Idx>, Rest...> : index<Idx> {};
}

template<typename... Args>
using expand = detail::expand<Args...>;

template<typename... Args>
using index_type = typename detail::index_type<Args...>::index_t;

}
}

#endif /* INCLUDE_BIT__HPP_ */
