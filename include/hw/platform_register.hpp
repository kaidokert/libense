#ifndef INCLUDE_HW_PLATFORM__REGISTER__HPP_
#define INCLUDE_HW_PLATFORM__REGISTER__HPP_

#include <stdint.h>
#include <type_traits>

namespace ense {

template<typename Bits, typename Inner, typename Value>
class PlatformRegister : public PlatformRegister<void, Inner, Value> {
	static_assert(
		std::is_same<
			typename std::underlying_type<Bits>::type,
			typename std::remove_cv<Value>::type>::value,
		"Bits is not a Value-enum");

	public:
		typedef Bits bits_type;

	public:
		bool get(Bits bit) const { return this->_value & static_cast<Value>(bit); }
};

template<typename Inner, typename Value>
class PlatformRegister<void, Inner, Value> {
	static_assert(std::is_integral<Value>::value, "");

	public:
		typedef void bits_type;
		typedef Value value_type;

	protected:
		Value _value;

	public:
		Value value() const { return _value; }
};

template<typename Bits, typename Inner, typename Value>
class WritablePlatformRegister : public PlatformRegister<Bits, Inner, Value> {
	private:
		static Value set_bits(Value value, Bits bit) {
			return value | static_cast<Value>(bit);
		}
		template<typename... Rest>
		static Value set_bits(Value value, Bits bit, Rest... rest) {
			return set_bits(set_bits(value, bit), rest...);
		}

		static Value clear_bits(Value value, Bits bit) {
			return value & ~static_cast<Value>(bit);
		}
		template<typename... Rest>
		static Value clear_bits(Value value, Bits bit, Rest... rest) {
			return clear_bits(clear_bits(value, bit), rest...);
		}

	public:
		template<typename... Flags>
		Inner& set(Flags... flags)
		{
			static_assert(std::is_base_of<WritablePlatformRegister, Inner>::value, "");
			static_cast<Inner*>(this)->value(set_bits(this->_value, flags...));
			return static_cast<Inner&>(*this);
		}

		template<typename... Flags>
		Inner& clear(Flags... flags)
		{
			static_assert(std::is_base_of<WritablePlatformRegister, Inner>::value, "");
			static_cast<Inner*>(this)->value(clear_bits(this->_value, flags...));
			return static_cast<Inner&>(*this);
		}

		void value(Value val)
		{
			this->_value = val;
		}
};

template<typename Inner, typename Value>
class WritablePlatformRegister<void, Inner, Value> : public PlatformRegister<void, Inner, Value> {
	public:
		void value(Value val)
		{
			this->_value = val;
		}
};

namespace traits {

template<typename Register>
constexpr bool is_platform_array_valid()
{
	static_assert(std::is_standard_layout<Register>::value, "Register array is not standard-layout");

	static_assert(std::extent<typename Register::value_type>::value > 0, "Register array value is no fixed-size array");

	static_assert(std::is_volatile<typename Register::value_type>::value, "Register array value is not volatile");

	static_assert(sizeof(Register) == sizeof(typename Register::value_type), "Register array contains superfluous fields");

	return true;
};

template<typename Register>
constexpr bool is_platform_register_valid()
{
	static_assert(
		std::is_base_of<
			ense::PlatformRegister<typename Register::bits_type, Register, typename Register::value_type>,
			Register>::value,
		"Register must inherit PlatformRegister<_, Register, _> somewhere");

	static_assert(std::is_standard_layout<Register>::value, "Register is not standard-layout");

	static_assert(std::is_volatile<typename Register::value_type>::value, "Register value is not volatile");

	static_assert(sizeof(Register) == sizeof(typename Register::value_type), "Register contains superfluous fields");

	return true;
};

}

template<typename Register>
using linker_placed_array = typename std::conditional<traits::is_platform_array_valid<Register>(), Register, void>::type;

template<typename Register>
using linker_placed_register = typename std::conditional<traits::is_platform_register_valid<Register>(), Register, void>::type;

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
