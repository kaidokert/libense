#define REGISTER_BIT_R(name) \
	bool name() const { return this->get(this_type::bits_type::name); }
#define REGISTER_BIT_W(name) \
	this_type& name(bool enable) \
	{ \
		if (enable) \
			this->set(this_type::bits_type::name); \
		else \
		   this->clear(this_type::bits_type::name); \
		return *this; \
	}
#define REGISTER_BIT_C(name) \
	this_type& name() \
	{ \
		this->set(this_type::bits_type::name); \
		return *this; \
	}
#define REGISTER_BIT_RW(name) \
	REGISTER_BIT_R(name) \
	REGISTER_BIT_W(name)

#define REGISTER_FIELD_R(type, name, ...) \
	type name() const \
	{ \
		typedef bit::expand<__VA_ARGS__> bp; \
		return static_cast<type>((this->value() & bp::mask) >> bp::begin); \
	}
#define REGISTER_FIELD_W(type, name, ...) \
	this_type& name(type value) \
	{ \
		typedef bit::expand<__VA_ARGS__> bp; \
		this->value((this->value() & ~bp::mask) | ((static_cast<uint32_t>(value) << bp::begin) & bp::mask)); \
		return *this; \
	}
#define REGISTER_FIELD_RW(type, name, ...) \
	REGISTER_FIELD_R(type, name, __VA_ARGS__) \
	REGISTER_FIELD_W(type, name, __VA_ARGS__)

#define REGISTER_ARRAY_R(array_type, name, ...) \
	std::remove_all_extents<array_type>::type name(size_t idx) const \
	{ \
		typedef std::remove_all_extents<array_type>::type value_type; \
		typedef bit::expand<__VA_ARGS__> bp; \
		static_assert(std::rank<array_type>::value == 1, "type must be one-dimensional"); \
		static_assert(bp::range == std::extent<array_type>::value * bp::width, "type does not fit range"); \
		uint32_t pos = bp::begin + idx * bp::width; \
		return static_cast<value_type>((this->value() >> pos) & bp::anchored_mask); \
	}
#define REGISTER_ARRAY_W(array_type, name, ...) \
	this_type& name(size_t idx, std::remove_all_extents<array_type>::type value) \
	{ \
		typedef std::remove_all_extents<array_type>::type value_type; \
		typedef bit::expand<__VA_ARGS__> bp; \
		static_assert(std::rank<array_type>::value == 1, "type must be one-dimensional"); \
		static_assert(bp::range == std::extent<array_type>::value * bp::width, "type does not fit range"); \
		uint32_t pos = bp::begin + idx * bp::width; \
		uint32_t mask = bp::anchored_mask << pos; \
		this->value((this->value() & ~mask) | ((static_cast<uint32_t>(value) & bp::anchored_mask) << pos)); \
		return *this; \
	}
#define REGISTER_ARRAY_RW(type, name, ...) \
	REGISTER_ARRAY_R(type, name, __VA_ARGS__) \
	REGISTER_ARRAY_W(type, name, __VA_ARGS__)

#define REGISTER_INT_R(name, ...) REGISTER_FIELD_R(uint32_t, name, __VA_ARGS__)
#define REGISTER_INT_W(name, ...) REGISTER_FIELD_W(uint32_t, name, __VA_ARGS__)
#define REGISTER_INT_RW(name, ...) \
	REGISTER_INT_R(name, __VA_ARGS__) \
	REGISTER_INT_W(name, __VA_ARGS__)
