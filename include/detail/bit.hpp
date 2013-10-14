#ifndef INCLUDE_DETAIL_BIT__HPP_D397B4F8F926519E
#define INCLUDE_DETAIL_BIT__HPP_D397B4F8F926519E

#include <mpl/list.hpp>

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



struct has_element_offsets {};

template<size_t... Offsets>
struct element_offsets : has_element_offsets {
	typedef ense::mpl::list<std::integral_constant<size_t, Offsets>...> offsets_t;
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

template<size_t... Offsets>
struct expand<element_offsets<Offsets...>> : element_offsets<Offsets...> {
	typedef typename expand::element_offsets::offsets_t offsets;
};



namespace detail {

	template<typename Bits>
	struct element_offset {
		template<size_t Width>
		struct element_offset_trivial {
			static constexpr size_t map(size_t idx)
			{
				return idx * Width;
			}
		};

		template<typename Offsets>
		struct element_offset_specified : element_offset_specified<typename Offsets::offsets_t> {
		};
		template<size_t... Offsets>
		struct element_offset_specified<ense::mpl::list<std::integral_constant<size_t, Offsets>...>> {
			template<size_t First, size_t... Rest, size_t ArgCount>
			static constexpr size_t nth(size_t idx, std::integral_constant<size_t, ArgCount>)
			{
				return idx == 0
					? First
					: nth<Rest...>(idx - 1, std::integral_constant<size_t, sizeof...(Rest)>());
			}

			template<size_t First>
			static constexpr size_t nth(size_t, std::integral_constant<size_t, 1>)
			{
				return First;
			}

			static constexpr size_t map(size_t idx)
			{
				return nth<Offsets...>(idx, std::integral_constant<size_t, sizeof...(Offsets)>());
			}
		};

		typedef typename std::conditional<
			std::is_base_of<has_element_offsets, Bits>::value,
			element_offset_specified<Bits>,
			element_offset_trivial<Bits::width>>::type type;
	};

}

template<typename Bits>
constexpr size_t element_offset(size_t index)
{
	return Bits::begin + detail::element_offset<Bits>::type::map(index);
}



template<typename Bits>
constexpr size_t splice_factor(size_t from_index, uint32_t mask)
{
	return mask
		? ((mask & 1) << element_offset<Bits>(from_index)) | splice_factor<Bits>(from_index + 1, mask >> 1)
		: 0;
}

template<typename Bits>
constexpr size_t splice_mask(size_t from_index, uint32_t mask)
{
	return splice_factor<Bits>(from_index, mask) * Bits::array_anchored_mask;
}

}
}
}

#endif /* INCLUDE_DETAIL_BIT__HPP_D397B4F8F926519E */
