#ifndef INCLUDE_HW_PLATFORM__REGISTER__HPP_
#define INCLUDE_HW_PLATFORM__REGISTER__HPP_

#include <stdint.h>
#include <type_traits>

namespace ense {

template<typename Bits, typename Inner, bool Volatile>
class PlatformRegister : public PlatformRegister<void, Inner, Volatile> {
	public:
		typedef Bits bits_type;

	public:
		bool get(Bits bit) const { return this->_value & static_cast<uint32_t>(bit); }
};

template<bool Volatile, typename Inner>
class PlatformRegister<void, Inner, Volatile> {
	protected:
		typename std::conditional<
			Volatile,
			volatile uint32_t,
			uint32_t>::type _value;

	public:
		uint32_t value() const { return _value; }
};

template<typename Bits, typename Inner, bool Volatile>
class WritablePlatformRegister : public PlatformRegister<Bits, Inner, Volatile> {
	private:
		static uint32_t set_bits(uint32_t value, Bits bit) {
			return value | static_cast<uint32_t>(bit);
		}
		template<typename... Rest>
		static uint32_t set_bits(uint32_t value, Bits bit, Rest... rest) {
			return set_bits(set_bits(value, bit), rest...);
		}

		static uint32_t clear_bits(uint32_t value, Bits bit) {
			return value & ~static_cast<uint32_t>(bit);
		}
		template<typename... Rest>
		static uint32_t clear_bits(uint32_t value, Bits bit, Rest... rest) {
			return clear_bits(clear_bits(value, bit), rest...);
		}

	public:
		template<typename... Flags>
		Inner& set(Flags... flags)
		{
			static_assert(std::is_base_of<WritablePlatformRegister, Inner>::value, "");
			this->_value = set_bits(this->_value, flags...);
			return static_cast<Inner&>(*this);
		}

		template<typename... Flags>
		Inner& clear(Flags... flags)
		{
			static_assert(std::is_base_of<WritablePlatformRegister, Inner>::value, "");
			this->_value = clear_bits(this->_value, flags...);
			return static_cast<Inner&>(*this);
		}

		void value(uint32_t val)
		{
			this->_value = val;
		}
};

template<bool Volatile, typename Inner>
class WritablePlatformRegister<void, Inner, Volatile> : public PlatformRegister<void, Inner, Volatile> {
	public:
		void value(uint32_t val)
		{
			this->_value = val;
		}
};

}

#ifndef __REGISTERS_NODEF
# include <hw/platform_register.hpp>
#endif


#elif !defined(__REGISTERS_UNDEF)


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
	}
#define REGISTER_BIT_RW(name) \
	REGISTER_BIT_R(name) \
	REGISTER_BIT_W(name)

#define MASK_RANGE(upper, lower) ((1 << ((upper) - (lower) + 1)) - 1)

#define REGISTER_INT_R(name, upper, lower) \
	uint32_t name() const { return (this->_value >> lower) & MASK_RANGE(upper, lower); }
#define REGISTER_INT_W(name, upper, lower) \
	auto name(uint32_t value) -> \
		decltype(*this) \
	{ \
		this->_value = (this->_value & ~(MASK_RANGE(upper, lower) << (lower))) (value << (lower)); \
		return *this; \
	}
#define REGISTER_INT_RW(name, upper, lower) \
	REGISTER_INT_R(name, upper, lower) \
	REGISTER_INT_W(name, upper, lower)


#else


#undef REGISTER_BIT_R
#undef REGISTER_BIT_W
#undef REGISTER_BIT_C
#undef REGISTER_BIT_RW
#undef MASK_RANGE
#undef REGISTER_INT_R
#undef REGISTER_INT_W
#undef REGISTER_INT_RW
#undef __REGISTERS_UNDEF


#endif /* INCLUDE_HW_PLATFORM__REGISTER__HPP_ */
