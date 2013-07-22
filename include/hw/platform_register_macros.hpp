#define REGISTER_BIT_R(name) \
	bool name() const { return this->get(std::decay<decltype(*this)>::type::bits_type::name); }
#define REGISTER_BIT_W(name) \
	auto name(bool enable) \
		-> typename std::decay<decltype(*this)>::type \
	{ \
		if (enable) \
			this->set(std::decay<decltype(*this)>::type::bits_type::name); \
		else \
		   this->clear(std::decay<decltype(*this)>::type::bits_type::name); \
		return *this; \
	}
#define REGISTER_BIT_C(name) \
	auto name() \
		-> typename std::decay<decltype(*this)>::type \
	{ \
		this->set(std::decay<decltype(*this)>::type::bits_type::name); \
		return *this; \
	}
#define REGISTER_BIT_T(name) \
	auto toggle_ ## name() \
		-> typename std::decay<decltype(*this)>::type \
	{ \
		return this->value(this->value() ^ static_cast<uint32_t>(std::decay<decltype(*this)>::type::bits_type::name)); \
	}
#define REGISTER_BIT_RW(name) \
	REGISTER_BIT_R(name) \
	REGISTER_BIT_W(name) \
	REGISTER_BIT_T(name)

#define REGISTER_FIELD_R(type, name, ...) \
	type name() const \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		return static_cast<type>((this->value() & bp::field_mask) >> bp::begin); \
	}
#define REGISTER_FIELD_W(field_type, name, ...) \
	auto name(field_type value) \
		-> typename std::decay<decltype(*this)>::type \
	{ \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		this->value((this->value() & ~bp::field_mask) | ((static_cast<uint32_t>(value) << bp::begin) & bp::field_mask)); \
		return *this; \
	}
#define REGISTER_FIELD_RW(type, name, ...) \
	REGISTER_FIELD_R(type, name, __VA_ARGS__) \
	REGISTER_FIELD_W(type, name, __VA_ARGS__)

#define REGISTER_ARRAY_R(array_type, name, ...) \
	std::remove_all_extents<array_type>::type name(detail::bit::index_type<__VA_ARGS__> idx) const \
	{ \
		typedef std::remove_all_extents<array_type>::type value_type; \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		static_assert(std::rank<array_type>::value == 1, "type must be one-dimensional"); \
		static_assert(bp::range == std::extent<array_type>::value * bp::width, "type does not fit range"); \
		uint32_t pos = bp::begin + static_cast<uint32_t>(idx) * bp::width; \
		return static_cast<value_type>((this->value() >> pos) & bp::array_anchored_mask); \
	}
#define REGISTER_ARRAY_W(array_type, name, ...) \
	auto name(detail::bit::index_type<__VA_ARGS__> idx, std::remove_all_extents<array_type>::type value) \
		-> typename std::decay<decltype(*this)>::type \
	{ \
		typedef std::remove_all_extents<array_type>::type value_type; \
		typedef detail::bit::expand<__VA_ARGS__> bp; \
		static_assert(std::rank<array_type>::value == 1, "type must be one-dimensional"); \
		static_assert(bp::range == std::extent<array_type>::value * bp::width, "type does not fit range"); \
		uint32_t pos = bp::begin + static_cast<uint32_t>(idx) * bp::width; \
		uint32_t mask = bp::array_anchored_mask << pos; \
		this->value((this->value() & ~mask) | ((static_cast<uint32_t>(value) << pos) & mask)); \
		return *this; \
	}
#define REGISTER_ARRAY_RW(type, name, ...) \
	REGISTER_ARRAY_R(type, name, __VA_ARGS__) \
	REGISTER_ARRAY_W(type, name, __VA_ARGS__)

#define REGISTER_SINGULAR_ARRAY_R(array_type, ...) \
	REGISTER_ARRAY_R(array_type, get, __VA_ARGS__) \
	REGISTER_ARRAY_R(array_type, operator[], __VA_ARGS__)
#define REGISTER_SINGULAR_ARRAY_W(array_type, ...) \
	REGISTER_ARRAY_W(array_type, set, __VA_ARGS__) \
	auto operator[](detail::bit::index_type<__VA_ARGS__> idx) \
		-> ense::detail::array_wrapper<typename std::decay<decltype(*this)>::type, detail::bit::index_type<__VA_ARGS__>, std::remove_all_extents<array_type>::type> \
	{ \
		return ense::detail::array_wrapper<typename std::decay<decltype(*this)>::type, detail::bit::index_type<__VA_ARGS__>, std::remove_all_extents<array_type>::type>(this, idx); \
	}
#define REGISTER_SINGULAR_ARRAY_RW(type, ...) \
	REGISTER_SINGULAR_ARRAY_R(type, __VA_ARGS__) \
	REGISTER_SINGULAR_ARRAY_W(type, __VA_ARGS__)

#define REGISTER_INT_R(name, ...) REGISTER_FIELD_R(uint32_t, name, __VA_ARGS__)
#define REGISTER_INT_W(name, ...) REGISTER_FIELD_W(uint32_t, name, __VA_ARGS__)
#define REGISTER_INT_RW(name, ...) \
	REGISTER_INT_R(name, __VA_ARGS__) \
	REGISTER_INT_W(name, __VA_ARGS__)
