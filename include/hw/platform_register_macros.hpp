#define ENSE_REGISTER_THIS_TYPE std::decay<decltype(*this)>::type
#define ENSE_REGISTER_COMMA ,

#define ENSE_REGISTER_BIT_R(_NAME) \
	bool _NAME() const \
	{ \
		return this->get(ENSE_REGISTER_THIS_TYPE::bits_type::_NAME); \
	}
#define ENSE_REGISTER_BIT_W(_NAME) \
	auto _NAME(bool enable) \
		-> decltype(*this) \
	{ \
		if (enable) \
			this->set(ENSE_REGISTER_THIS_TYPE::bits_type::_NAME); \
		else \
		   this->clear(ENSE_REGISTER_THIS_TYPE::bits_type::_NAME); \
		return *this; \
	}
#define ENSE_REGISTER_BIT_C(_NAME) \
	auto _NAME() \
		-> decltype(*this) \
	{ \
		this->set(ENSE_REGISTER_THIS_TYPE::bits_type::_NAME); \
		return *this; \
	}
#define ENSE_REGISTER_BIT_T(_NAME) \
	auto toggle_ ## _NAME() \
		-> decltype(*this) \
	{ \
		return this->value(this->value() ^ static_cast<uint32_t>(ENSE_REGISTER_THIS_TYPE::bits_type::_NAME)); \
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
		return static_cast<_TYPE>((this->value() & bp::field_mask) >> bp::begin); \
	}
#define ENSE_REGISTER_FIELD_W(_TYPE, _NAME, ...) \
	auto _NAME(_TYPE value) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		this->value((this->value() & ~bp::field_mask) | ((static_cast<uint32_t>(value) << bp::begin) & bp::field_mask)); \
		return *this; \
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
	static_assert(__VA_ARGS__::range == std::extent<_TYPE>::value * __VA_ARGS__::width, "type does not fit range");
#define ENSE_REGISTER_ARRAY_STATIC_ASSERT(_TYPE, ...) \
	ENSE_REGISTER_ARRAY_STATIC_ASSERT_IMPL(_TYPE, detail::bit::expand<__VA_ARGS__>)
#define ENSE_REGISTER_ARRAY_R(_TYPE, _NAME, ...) \
	ENSE_REGISTER_ARRAY_STATIC_ASSERT(_TYPE, __VA_ARGS__) \
	std::remove_all_extents<_TYPE>::type _NAME(uint32_t idx) const \
	{ \
		typedef std::remove_all_extents<_TYPE>::type value_type; \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		uint32_t pos = bp::begin + idx * bp::width; \
		return static_cast<value_type>((this->value() >> pos) & bp::array_anchored_mask); \
	}
#define ENSE_REGISTER_ARRAY_W(_TYPE, _NAME, _VALUE_DECL, _VALUE, ...) \
	ENSE_REGISTER_ARRAY_STATIC_ASSERT(_TYPE, __VA_ARGS__) \
	auto _NAME(uint32_t idx _VALUE_DECL) \
		-> decltype(*this) \
	{ \
		typedef std::remove_all_extents<_TYPE>::type value_type; \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		uint32_t pos = bp::begin + idx * bp::width; \
		uint32_t mask = bp::array_anchored_mask << pos; \
		this->value((this->value() & ~mask) | (static_cast<uint32_t>(_VALUE) << pos)); \
		return *this; \
	} \
	auto _NAME ## _mask(uint32_t idx_mask _VALUE_DECL) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		uint32_t mask = idx_mask; \
		uint32_t splice_mask = 0; \
		uint32_t splice_value = 0; \
		if (bp::width == 1) { \
			splice_mask = bp::field_anchored_mask << bp::begin; \
			splice_value = (static_cast<uint32_t>(_VALUE) * mask) << bp::begin; \
		} else { \
			uint32_t offset = bp::begin; \
			while (mask) { \
				if (mask & 1) { \
					splice_mask |= bp::array_anchored_mask << offset; \
					splice_value |= static_cast<uint32_t>(_VALUE) << offset; \
				} \
				offset += bp::width; \
				mask >>= 1; \
			} \
		} \
		this->value((this->value() & ~splice_mask) | (splice_value & splice_mask)); \
		return *this; \
	}
#define ENSE_REGISTER_ARRAY_W_TEMPLATES(_TYPE, _NAME, _VALUE_DECL, _VALUE, ...) \
	ENSE_REGISTER_ARRAY_STATIC_ASSERT(_TYPE, __VA_ARGS__) \
	template<uint32_t Mask> \
	auto _NAME ## _mask(_VALUE_DECL) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		static_assert(Mask >> std::extent<_TYPE>::value == 0, "Mask invalid"); \
		constexpr uint32_t splice_factor = detail::bit::splice_factor(0, bp::width, Mask); \
		constexpr uint32_t splice_mask = detail::bit::splice_mask(0, bp::width, Mask) << bp::begin; \
		uint32_t splice_value = (splice_factor * static_cast<uint32_t>(_VALUE)) << bp::begin; \
		this->value((this->value() & ~splice_mask) | splice_value); \
		return *this; \
	} \
	template<uint32_t Bound1, uint32_t Bound2> \
	auto _NAME ## _range(_VALUE_DECL) \
		-> decltype(*this) \
	{ \
		typedef detail::bit::expand<detail::bit::range<Bound1, Bound2>> range; \
		static_assert(range::end < std::extent<_TYPE>::value, "Index out or range"); \
		return _NAME ## _mask<range::field_mask>(_VALUE); \
	}

#define REGISTER_ARRAY_R_PLAIN ENSE_REGISTER_ARRAY_R
#define REGISTER_ARRAY_W_PLAIN(_TYPE, _NAME, ...) \
	ENSE_REGISTER_ARRAY_W(_TYPE, _NAME, ENSE_REGISTER_COMMA std::remove_all_extents<_TYPE>::type value, value, __VA_ARGS__) \
	ENSE_REGISTER_ARRAY_W_TEMPLATES(_TYPE, _NAME, std::remove_all_extents<_TYPE>::type value, value, __VA_ARGS__)


#define REGISTER_ARRAY_R(_TYPE, _NAME, ...) \
	static constexpr size_t _NAME ## _extent = std::extent<_TYPE>::value; \
	REGISTER_ARRAY_R_PLAIN(_TYPE, _NAME, __VA_ARGS__)
#define REGISTER_ARRAY_W(_TYPE, _NAME, ...) \
	static constexpr size_t _NAME ## _extent = std::extent<_TYPE>::value; \
	REGISTER_ARRAY_W_PLAIN(_TYPE, _NAME, __VA_ARGS__)
#define REGISTER_ARRAY_C(_TYPE, _NAME, ...) \
	static constexpr size_t _NAME ## _extent = std::extent<_TYPE>::value; \
	ENSE_REGISTER_ARRAY_W(_TYPE, _NAME, , 1, __VA_ARGS__) \
	ENSE_REGISTER_ARRAY_W_TEMPLATES(_TYPE, _NAME, , 1, __VA_ARGS__)
#define REGISTER_ARRAY_RW(_TYPE, _NAME, ...) \
	static constexpr size_t _NAME ## _extent = std::extent<_TYPE>::value; \
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
