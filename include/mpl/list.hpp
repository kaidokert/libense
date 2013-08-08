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

	template<typename ListA, typename List>
	struct concat;

	template<typename... ListA, typename... ListB>
	struct concat<list<ListA...>, list<ListB...>> {
		typedef list<ListA..., ListB...> type;
	};
}

template<typename ListA, typename ListB>
using concat = typename detail::concat<ListA, ListB>::type;

template<typename Head, typename List>
using cons = concat<list<Head>, List>;




namespace detail {

	template<typename T, typename Length, T Offset>
	struct make_range {
		typedef mpl::concat<
			typename make_range<T, std::integral_constant<T, (Length::value + 0) / 2>, Offset>::type,
			typename make_range<T, std::integral_constant<T, (Length::value + 1) / 2>, Offset + (Length::value + 0) / 2>::type
			> type;
	};

	template<typename T, T Offset>
	struct make_range<T, std::integral_constant<T, 0>, Offset> {
		typedef list<> type;
	};

	template<typename T, T Offset>
	struct make_range<T, std::integral_constant<T, 1>, Offset> {
		typedef list<std::integral_constant<T, Offset>> type;
	};

}

template<typename T, T Begin, T End>
using make_range = typename detail::make_range<T, std::integral_constant<T, End - Begin + 1>, Begin>::type;




namespace detail {

	template<typename Fn, typename List>
	struct map {
		static_assert(std::is_same<List, list<>>::value, "");
		typedef list<> type;
	};

	template<typename Fn, typename... Items>
	struct map<Fn, list<Items...>> {
		typedef list<typename Fn::template apply<Items>::type...> type;
	};

}

template<typename Fn, typename List>
using map = typename detail::map<Fn, List>::type;




namespace detail {

	template<typename Fn, typename Acc, typename List>
	struct fold_left {
		static_assert(std::is_same<List, list<>>::value, "");
		typedef Acc type;
	};

	template<typename Fn, typename Acc, typename Head, typename... Tail>
	struct fold_left<Fn, Acc, list<Head, Tail...>> {
		typedef typename fold_left<Fn, typename Fn::template apply<Acc, Head>::type, list<Tail...>>::type type;
	};

}

template<typename Fn, typename Acc, typename List>
using fold_left = typename detail::fold_left<Fn, Acc, List>::type;




namespace detail {

	template<typename Fn, typename List>
	struct partition {
		static_assert(std::is_same<List, list<>>::value, "");
		typedef list<> left;
		typedef list<> right;
	};

	template<typename Fn, typename Head, typename... Tail>
	struct partition<Fn, list<Head, Tail...>> {
		static constexpr bool is_left = Fn::template apply<Head>::value;

		typedef partition<Fn, list<Tail...>> rest_part;
		typedef typename std::conditional<
			is_left,
			mpl::cons<Head, typename rest_part::left>,
			typename rest_part::left
			>::type left;
		typedef typename std::conditional<
			!is_left,
			mpl::cons<Head, typename rest_part::right>,
			typename rest_part::right
			>::type right;
	};

}

template<typename Fn, typename List>
using partition = typename detail::partition<Fn, List>;

}
}

#endif /* INCLUDE_MPL_LIST__HPP_092B77D39F5C1127 */
