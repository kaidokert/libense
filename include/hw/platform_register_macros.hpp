#define ENSE_REGISTER_THIS_TYPE std::decay<decltype(*this)>::type
#define ENSE_REGISTER_COMMA ,

#define REGISTER_BIT_R(BIT_NAME) \
	bool BIT_NAME() const \
	{ \
		return this->has_all(ENSE_REGISTER_THIS_TYPE::bits_type::BIT_NAME); \
	}
#define REGISTER_BIT_W(BIT_NAME) \
	auto BIT_NAME(bool enable) \
		-> decltype(*this) \
	{ \
		if (enable) \
			return this->set(ENSE_REGISTER_THIS_TYPE::bits_type::BIT_NAME); \
		else \
			return this->clear(ENSE_REGISTER_THIS_TYPE::bits_type::BIT_NAME); \
	}
#define REGISTER_BIT_C(BIT_NAME) \
	auto BIT_NAME() \
		-> decltype(*this) \
	{ \
		return this->set(ENSE_REGISTER_THIS_TYPE::bits_type::BIT_NAME); \
	}
#define REGISTER_BIT_T(BIT_NAME) \
	auto toggle_ ## BIT_NAME() \
		-> decltype(*this) \
	{ \
		return this->toggle(ENSE_REGISTER_THIS_TYPE::bits_type::BIT_NAME); \
	}
#define REGISTER_BIT_RW(BIT_NAME) \
	REGISTER_BIT_R(BIT_NAME) \
	REGISTER_BIT_W(BIT_NAME) \
	REGISTER_BIT_T(BIT_NAME)



#define REGISTER_FIELD_R(FIELD_TYPE, FIELD_NAME, ...) \
	FIELD_TYPE FIELD_NAME() const \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		return static_cast<FIELD_TYPE>(this->template bits<bp::begin, bp::end>()); \
	}
#define REGISTER_FIELD_W(FIELD_TYPE, FIELD_NAME, ...) \
	auto FIELD_NAME(FIELD_TYPE value) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		return this->template bits<bp::begin, bp::end>(static_cast<typename ENSE_REGISTER_THIS_TYPE::word_type>(value)); \
	}
#define REGISTER_FIELD_RW(FIELD_TYPE, FIELD_NAME, ...) \
	REGISTER_FIELD_R(FIELD_TYPE, FIELD_NAME, __VA_ARGS__) \
	REGISTER_FIELD_W(FIELD_TYPE, FIELD_NAME, __VA_ARGS__)



#define REGISTER_INT_R(FIELD_NAME, ...) REGISTER_FIELD_R(uint32_t, FIELD_NAME, __VA_ARGS__)
#define REGISTER_INT_W(FIELD_NAME, ...) REGISTER_FIELD_W(uint32_t, FIELD_NAME, __VA_ARGS__)
#define REGISTER_INT_RW(FIELD_NAME, ...) \
	REGISTER_INT_R(FIELD_NAME, __VA_ARGS__) \
	REGISTER_INT_W(FIELD_NAME, __VA_ARGS__)



#define ENSE_REGISTER_ARRAY_R(FIELD_TYPE, FIELD_NAME, ...) \
	std::remove_all_extents<FIELD_TYPE>::type FIELD_NAME(uint32_t idx) const \
	{ \
		typedef std::remove_all_extents<FIELD_TYPE>::type value_type; \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		traits::assert_platform_array_type<FIELD_TYPE, bp>(); \
		uint32_t pos = detail::bit::element_offset<bp>(idx); \
		return static_cast<value_type>(this->bits(pos, pos + bp::width - 1)); \
	}
#define ENSE_REGISTER_ARRAY_W(FIELD_TYPE, FIELD_NAME, VALUE_DECL, VALUE, ...) \
	auto FIELD_NAME(uint32_t idx VALUE_DECL) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		traits::assert_platform_array_type<FIELD_TYPE, bp>(); \
		uint32_t pos = detail::bit::element_offset<bp>(idx % std::extent<FIELD_TYPE>::value); \
		return this->bits(pos, pos + bp::width - 1, static_cast<typename ENSE_REGISTER_THIS_TYPE::word_type>(VALUE)); \
	}
#define ENSE_REGISTER_ARRAY_W_TEMPLATES(FIELD_TYPE, FIELD_NAME, VALUE_DECL, VCOMMA, VALUE, ...) \
	auto FIELD_NAME ## _list(VALUE_DECL VCOMMA mpl::list<>) \
		-> decltype(*this) \
	{ \
		(void) VALUE; \
		return *this; \
	} \
	template<uint32_t... Items> \
	auto FIELD_NAME ## _list(VALUE_DECL VCOMMA mpl::list<std::integral_constant<uint32_t, Items>...>) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		traits::assert_platform_array_type<FIELD_TYPE, bp>(); \
		static_assert(mpl::all((Items < std::extent<FIELD_TYPE>::value)...), "Items list invalid"); \
		constexpr uint32_t min_pos = mpl::min(detail::bit::element_offset<bp>(Items)...); \
		constexpr uint32_t width = ENSE_REGISTER_THIS_TYPE::width; \
		constexpr uint32_t word = min_pos / width; \
		typedef mpl::partition< \
			mpl::div_equals<uint32_t, width, min_pos / width>::template fn, \
			mpl::list< \
				std::integral_constant< \
					uint32_t, \
					detail::bit::element_offset<bp>(Items)>...>> items_split; \
		constexpr uint32_t splice_factor = detail::bit::splice_factor<bp>(min_pos, typename items_split::left()); \
		constexpr uint32_t splice_mask = detail::bit::splice_mask<bp>(min_pos, typename items_split::left()); \
		auto splice_value = (static_cast<typename ENSE_REGISTER_THIS_TYPE::word_type>(VALUE) & bp::element_mask) * splice_factor; \
		this->word(word, (this->word(word) & ~splice_mask) | splice_value); \
		return this->FIELD_NAME ## _list(VALUE, typename items_split::right()); \
	} \
	template<uint32_t... Items> \
	auto FIELD_NAME ## _list(VALUE_DECL) \
		-> decltype(*this) \
	{ \
		return FIELD_NAME ## _list(VALUE, mpl::list<std::integral_constant<uint32_t, Items>...>()); \
	} \
	template<uint32_t Bound1, uint32_t Bound2> \
	auto FIELD_NAME ## _range(VALUE_DECL) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<detail::bit::range<Bound1, Bound2>> range; \
		static_assert(range::end < std::extent<FIELD_TYPE>::value, "Index out of range"); \
		return FIELD_NAME ## _list(VALUE, typename range::items_list()); \
	}



#define REGISTER_ARRAY_R(FIELD_TYPE, FIELD_NAME, ...) \
	ENSE_REGISTER_ARRAY_R(FIELD_TYPE, FIELD_NAME, __VA_ARGS__)
#define REGISTER_ARRAY_W(FIELD_TYPE, FIELD_NAME, ...) \
	ENSE_REGISTER_ARRAY_W(FIELD_TYPE, FIELD_NAME, ENSE_REGISTER_COMMA std::remove_all_extents<FIELD_TYPE>::type value, value, __VA_ARGS__) \
	ENSE_REGISTER_ARRAY_W_TEMPLATES(FIELD_TYPE, FIELD_NAME, std::remove_all_extents<FIELD_TYPE>::type value, ENSE_REGISTER_COMMA, value, __VA_ARGS__)
#define REGISTER_ARRAY_C(FIELD_TYPE, FIELD_NAME, ...) \
	ENSE_REGISTER_ARRAY_W(FIELD_TYPE, FIELD_NAME, , 1, __VA_ARGS__) \
	ENSE_REGISTER_ARRAY_W_TEMPLATES(FIELD_TYPE, FIELD_NAME, , , 1, __VA_ARGS__)
#define REGISTER_ARRAY_RW(FIELD_TYPE, FIELD_NAME, ...) \
	REGISTER_ARRAY_R(FIELD_TYPE, FIELD_NAME, __VA_ARGS__) \
	REGISTER_ARRAY_W(FIELD_TYPE, FIELD_NAME, __VA_ARGS__)


#define REGISTER_SINGULAR_ARRAY_R(FIELD_TYPE, ...) \
	REGISTER_ARRAY_R(FIELD_TYPE, get, __VA_ARGS__) \
	REGISTER_ARRAY_R(FIELD_TYPE, operator[], __VA_ARGS__)
#define REGISTER_SINGULAR_ARRAY_W(FIELD_TYPE, ...) \
	REGISTER_ARRAY_W(FIELD_TYPE, set, __VA_ARGS__) \
	auto operator[](uint32_t idx) \
		-> ense::detail::array_wrapper<typename ENSE_REGISTER_THIS_TYPE, std::remove_all_extents<FIELD_TYPE>::type> \
	{ \
		return ense::detail::array_wrapper<typename ENSE_REGISTER_THIS_TYPE, std::remove_all_extents<FIELD_TYPE>::type>(this, idx); \
	}
#define REGISTER_SINGULAR_ARRAY_RW(FIELD_TYPE, ...) \
	REGISTER_SINGULAR_ARRAY_R(FIELD_TYPE, __VA_ARGS__) \
	REGISTER_SINGULAR_ARRAY_W(FIELD_TYPE, __VA_ARGS__)
