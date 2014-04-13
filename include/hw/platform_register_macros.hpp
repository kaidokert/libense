#define ENSE_REGISTER_THIS_TYPE std::decay<decltype(*this)>::type
#define ENSE_REGISTER_COMMA ,

#define ENSE_REGISTER_BIT_R(_NAME) \
	bool _NAME() const \
	{ \
		return this->has_all(ENSE_REGISTER_THIS_TYPE::bits_type::_NAME); \
	}
#define ENSE_REGISTER_BIT_W(_NAME) \
	auto _NAME(bool enable) \
		-> decltype(*this) \
	{ \
		if (enable) \
			return this->set(ENSE_REGISTER_THIS_TYPE::bits_type::_NAME); \
		else \
			return this->clear(ENSE_REGISTER_THIS_TYPE::bits_type::_NAME); \
	}
#define ENSE_REGISTER_BIT_C(_NAME) \
	auto _NAME() \
		-> decltype(*this) \
	{ \
		return this->set(ENSE_REGISTER_THIS_TYPE::bits_type::_NAME); \
	}
#define ENSE_REGISTER_BIT_T(_NAME) \
	auto toggle_ ## _NAME() \
		-> decltype(*this) \
	{ \
		return this->toggle(ENSE_REGISTER_THIS_TYPE::bits_type::_NAME); \
	}

#define REGISTER_BIT_R ENSE_REGISTER_BIT_R
#define REGISTER_BIT_W ENSE_REGISTER_BIT_W
#define REGISTER_BIT_C ENSE_REGISTER_BIT_C
#define REGISTER_BIT_T ENSE_REGISTER_BIT_T
#define REGISTER_BIT_RW(...) \
	REGISTER_BIT_R(__VA_ARGS__) \
	REGISTER_BIT_W(__VA_ARGS__) \
	REGISTER_BIT_T(__VA_ARGS__)



#define ENSE_REGISTER_FIELD_R(_TYPE, _NAME, ...) \
	_TYPE _NAME() const \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		return static_cast<_TYPE>(this->template bits<bp::begin, bp::end>()); \
	}
#define ENSE_REGISTER_FIELD_W(_TYPE, _NAME, ...) \
	auto _NAME(_TYPE value) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		return this->template bits<bp::begin, bp::end>(static_cast<typename ENSE_REGISTER_THIS_TYPE::word_type>(value)); \
	}

#define REGISTER_FIELD_R ENSE_REGISTER_FIELD_R
#define REGISTER_FIELD_W ENSE_REGISTER_FIELD_W
#define REGISTER_FIELD_RW(...) \
	REGISTER_FIELD_R(__VA_ARGS__) \
	REGISTER_FIELD_W(__VA_ARGS__)



#define REGISTER_INT_R(...) REGISTER_FIELD_R(uint32_t, __VA_ARGS__)
#define REGISTER_INT_W(...) REGISTER_FIELD_W(uint32_t, __VA_ARGS__)
#define REGISTER_INT_RW(...) \
	REGISTER_INT_R(__VA_ARGS__) \
	REGISTER_INT_W(__VA_ARGS__)



#define ENSE_REGISTER_ARRAY_STATIC_ASSERT_IMPL(_TYPE, ...) \
	static_assert(std::rank<_TYPE>::value == 1, "type must be one-dimensional"); \
	static_assert(std::is_base_of<detail::bit::has_element_offsets, __VA_ARGS__>::value \
			|| __VA_ARGS__::range == std::extent<_TYPE>::value * __VA_ARGS__::width, "type does not fit range");
#define ENSE_REGISTER_ARRAY_STATIC_ASSERT(_TYPE, ...) \
	ENSE_REGISTER_ARRAY_STATIC_ASSERT_IMPL(_TYPE, detail::bit::expand<__VA_ARGS__>)
#define ENSE_REGISTER_ARRAY_R(_TYPE, _NAME, ...) \
	ENSE_REGISTER_ARRAY_STATIC_ASSERT(_TYPE, __VA_ARGS__) \
	std::remove_all_extents<_TYPE>::type _NAME(uint32_t idx) const \
	{ \
		typedef std::remove_all_extents<_TYPE>::type value_type; \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		uint32_t pos = detail::bit::element_offset<bp>(idx % std::extent<_TYPE>::value); \
		return static_cast<value_type>(this->bits(pos, pos + bp::width - 1)); \
	}
#define ENSE_REGISTER_ARRAY_W(_TYPE, _NAME, _VALUE_DECL, _VALUE, ...) \
	ENSE_REGISTER_ARRAY_STATIC_ASSERT(_TYPE, __VA_ARGS__) \
	auto _NAME(uint32_t idx _VALUE_DECL) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		uint32_t pos = detail::bit::element_offset<bp>(idx % std::extent<_TYPE>::value); \
		return this->bits(pos, pos + bp::width - 1, static_cast<typename ENSE_REGISTER_THIS_TYPE::word_type>(_VALUE)); \
	}
#define ENSE_REGISTER_ARRAY_W_TEMPLATES(_TYPE, _NAME, _VALUE_DECL, _VCOMMA, _VALUE, ...) \
	ENSE_REGISTER_ARRAY_STATIC_ASSERT(_TYPE, __VA_ARGS__) \
	auto _NAME ## _list(_VALUE_DECL _VCOMMA mpl::list<>) \
		-> decltype(*this) \
	{ \
		(void) _VALUE; \
		return *this; \
	} \
	template<uint32_t... Items> \
	auto _NAME ## _list(_VALUE_DECL _VCOMMA mpl::list<std::integral_constant<uint32_t, Items>...>) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		static_assert(mpl::all((Items < std::extent<_TYPE>::value)...), "Items list invalid"); \
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
		auto splice_value = (static_cast<typename ENSE_REGISTER_THIS_TYPE::word_type>(_VALUE) & bp::element_mask) * splice_factor; \
		this->word(word, (this->word(word) & ~splice_mask) | splice_value); \
		return this->_NAME ## _list(_VALUE, typename items_split::right()); \
	} \
	template<uint32_t... Items> \
	auto _NAME ## _list(_VALUE_DECL) \
		-> decltype(*this) \
	{ \
		return _NAME ## _list(_VALUE, mpl::list<std::integral_constant<uint32_t, Items>...>()); \
	} \
	template<uint32_t Bound1, uint32_t Bound2> \
	auto _NAME ## _range(_VALUE_DECL) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<detail::bit::range<Bound1, Bound2>> range; \
		static_assert(range::end < std::extent<_TYPE>::value, "Index out of range"); \
		return _NAME ## _list(_VALUE, typename range::items_list()); \
	}

#define REGISTER_ARRAY_R_PLAIN ENSE_REGISTER_ARRAY_R
#define REGISTER_ARRAY_W_PLAIN(_TYPE, _NAME, ...) \
	ENSE_REGISTER_ARRAY_W(_TYPE, _NAME, ENSE_REGISTER_COMMA std::remove_all_extents<_TYPE>::type value, value, __VA_ARGS__) \
	ENSE_REGISTER_ARRAY_W_TEMPLATES(_TYPE, _NAME, std::remove_all_extents<_TYPE>::type value, ENSE_REGISTER_COMMA, value, __VA_ARGS__)


#define REGISTER_ARRAY_R(_TYPE, _NAME, ...) \
	REGISTER_ARRAY_R_PLAIN(_TYPE, _NAME, __VA_ARGS__)
#define REGISTER_ARRAY_W(_TYPE, _NAME, ...) \
	REGISTER_ARRAY_W_PLAIN(_TYPE, _NAME, __VA_ARGS__)
#define REGISTER_ARRAY_C(_TYPE, _NAME, ...) \
	ENSE_REGISTER_ARRAY_W(_TYPE, _NAME, , 1, __VA_ARGS__) \
	ENSE_REGISTER_ARRAY_W_TEMPLATES(_TYPE, _NAME, , , 1, __VA_ARGS__)
#define REGISTER_ARRAY_RW(_TYPE, _NAME, ...) \
	REGISTER_ARRAY_R_PLAIN(_TYPE, _NAME, __VA_ARGS__) \
	REGISTER_ARRAY_W_PLAIN(_TYPE, _NAME, __VA_ARGS__)


#define REGISTER_SINGULAR_ARRAY_R(_TYPE, ...) \
	REGISTER_ARRAY_R(_TYPE, get, __VA_ARGS__) \
	REGISTER_ARRAY_R_PLAIN(_TYPE, operator[], __VA_ARGS__)
#define REGISTER_SINGULAR_ARRAY_W(_TYPE, ...) \
	REGISTER_ARRAY_W(_TYPE, set, __VA_ARGS__) \
	auto operator[](uint32_t idx) \
		-> ense::detail::array_wrapper<typename ENSE_REGISTER_THIS_TYPE, std::remove_all_extents<_TYPE>::type> \
	{ \
		return ense::detail::array_wrapper<typename ENSE_REGISTER_THIS_TYPE, std::remove_all_extents<_TYPE>::type>(this, idx); \
	}
#define REGISTER_SINGULAR_ARRAY_C(_TYPE, ...) \
	REGISTER_ARRAY_C(_TYPE, clear, __VA_ARGS__)
#define REGISTER_SINGULAR_ARRAY_RW(_TYPE, ...) \
	REGISTER_SINGULAR_ARRAY_R(_TYPE, __VA_ARGS__) \
	REGISTER_SINGULAR_ARRAY_W(_TYPE, __VA_ARGS__)
