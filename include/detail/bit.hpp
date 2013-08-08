#ifndef INCLUDE_DETAIL_BIT__HPP_D397B4F8F926519E
#define INCLUDE_DETAIL_BIT__HPP_D397B4F8F926519E

namespace ense {
namespace detail {
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



struct has_index_tag {};

template<typename Idx>
struct index : has_index_tag {
	typedef Idx index_t;
};



template<typename... Args>
struct expand : expand<Args>... {};

template<typename Mismatch>
struct expand<Mismatch>;

template<size_t Bound1, size_t Bound2>
struct expand<range<Bound1, Bound2>> : range<Bound1, Bound2> {
	static constexpr uint32_t begin = expand::begin_t::value;
	static constexpr uint32_t end = expand::end_t::value;

	static_assert(end < 32, "Upper >= 32");
	static_assert(begin <= end, "Lower > Upper");

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
using index_type = typename std::conditional<
	std::is_base_of<has_index_tag, expand<Args...>>::value,
	expand<Args...>,
	index<uint32_t>
	>::type::index_t;




constexpr uint32_t splice_factor(uint32_t first_bit, uint32_t bit_width, uint32_t mask)
{
	return mask
		? ((mask & 1) << first_bit) | splice_factor(first_bit + bit_width, bit_width, mask >> 1)
		: 0;
}

constexpr uint32_t splice_mask(uint32_t first_bit, uint32_t bit_width, uint32_t mask)
{
	return mask
		? ((mask & 1 ? (1U << bit_width) - 1 : 0) << first_bit) | splice_mask(first_bit + bit_width, bit_width, mask >> 1)
		: 0;
}

}
}
}

#endif /* INCLUDE_DETAIL_BIT__HPP_D397B4F8F926519E */
