#define REGISTER_BIT_R(name) \
	bool name() const { return this->get(this_type::bits_type::name); }
#define REGISTER_BIT_W(name) \
	auto name(bool enable) -> \
		decltype(*this) \
	{ \
		if (enable) \
			this->set(this_type::bits_type::name); \
		else \
		   this->clear(this_type::bits_type::name); \
		return *this; \
	}
#define REGISTER_BIT_C(name) \
	auto name() -> \
		decltype(*this) \
	{ \
		this->set(this_type::bits_type::name); \
		return *this; \
	}
#define REGISTER_BIT_RW(name) \
	REGISTER_BIT_R(name) \
	REGISTER_BIT_W(name)

#define MASK_RANGE(upper, lower) ((1 << ((upper) - (lower) + 1)) - 1)

#define REGISTER_INT_R(name, upper, lower) \
	uint32_t name() const { return (this->_value >> lower) & MASK_RANGE((upper), (lower)); }
#define REGISTER_INT_W(name, upper, lower) \
	auto name(uint32_t value) -> \
		decltype(*this) \
	{ \
		this->value((this->_value & ~(MASK_RANGE((upper), (lower)) << (lower))) | ((value & MASK_RANGE((upper), (lower))) << (lower))); \
		return *this; \
	}
#define REGISTER_INT_RW(name, upper, lower) \
	REGISTER_INT_R(name, upper, lower) \
	REGISTER_INT_W(name, upper, lower)
