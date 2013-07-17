#ifndef INCLUDE_MPL_LIST__HPP_
#define INCLUDE_MPL_LIST__HPP_

#include <type_traits>

namespace ense {
namespace mpl {


template<typename... Items>
struct list {};




namespace detail {

	template<typename Head, typename Tail>
	struct cons {
		static_assert(std::is_same<Tail, list<>>::value, "");
		typedef list<Head> type;
	};

	template<typename Head, typename... Tail>
	struct cons<Head, list<Tail...>> {
		typedef list<Head, Tail...> type;
	};

}

template<typename Head, typename List>
using cons = typename detail::cons<Head, List>::type;




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

#endif /* INCLUDE_MPL_LIST__HPP_ */
