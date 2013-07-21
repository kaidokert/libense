#ifndef INCLUDE_HW_PLATFORM__REGISTER__HPP_
#define INCLUDE_HW_PLATFORM__REGISTER__HPP_

#include <stdint.h>
#include <type_traits>
#include <tuple>

#include <bit.hpp>

namespace ense {

namespace detail {

	template<typename Target, typename Index, typename Value>
	struct const_array_wrapper {
		Target* target;
		Index idx;

		const_array_wrapper(Target* target, Index idx)
			: target(target), idx(idx)
		{
		}

		operator Value() const
		{
			return target->get(idx);
		}

		template<typename T>
		operator T() const
		{
			return static_cast<T>(target->get(idx));
		}

	};

	template<typename Target, typename Index, typename Value>
	struct array_wrapper : const_array_wrapper<Target, Index, Value> {
		array_wrapper(Target* target, Index idx)
			: const_array_wrapper<Target, Index, Value>(target, idx)
		{
		}

		Value operator=(Value val)
		{
			this->target->set(this->idx, val);
			return val;
		}
	};

}

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
		bool get(Bits bit) const
		{
			return this->_value & static_cast<typename PlatformRegister::value_type>(bit);
		}
};

template<typename Inner, typename Value>
class PlatformRegister<void, Inner, Value> {
	static_assert(std::is_integral<Value>::value, "");

	public:
		typedef void bits_type;
		typedef Value memory_type;
		typedef typename std::remove_cv<memory_type>::type value_type;

	protected:
		Value _value;

	public:
		value_type value() const { return _value; }
};

template<typename Bits, typename Inner, typename Value>
class WritablePlatformRegister : public PlatformRegister<Bits, Inner, Value> {
	public:
		template<typename... Flags>
		Inner& set(Flags... flags)
		{
			static_assert(std::is_base_of<WritablePlatformRegister, Inner>::value, "");
			Inner* self = static_cast<Inner*>(this);
			self->value(self->value() | bit::mask<typename WritablePlatformRegister::value_type>(flags...));
			return *self;
		}

		template<typename... Flags>
		Inner& clear(Flags... flags)
		{
			static_assert(std::is_base_of<WritablePlatformRegister, Inner>::value, "");
			Inner* self = static_cast<Inner*>(this);
			self->value(self->value() & ~bit::mask<typename WritablePlatformRegister::value_type>(flags...));
			return *self;
		}

		typename WritablePlatformRegister::value_type value() const { return this->_value; }

		Inner& value(typename WritablePlatformRegister::value_type val)
		{
			static_assert(std::is_base_of<WritablePlatformRegister, Inner>::value, "");
			this->_value = val;
			return static_cast<Inner&>(*this);
		}
};

template<typename Inner, typename Value>
class WritablePlatformRegister<void, Inner, Value> : public PlatformRegister<void, Inner, Value> {
	public:
		typename WritablePlatformRegister::value_type value() const { return this->_value; }

		Inner& value(typename WritablePlatformRegister::value_type val)
		{
			static_assert(std::is_base_of<WritablePlatformRegister, Inner>::value, "");
			this->_value = val;
			return static_cast<Inner&>(*this);
		}
};

namespace traits {

	template<typename Register>
	constexpr bool is_platform_array_valid()
	{
		static_assert(std::is_standard_layout<Register>::value, "Register array is not standard-layout");

		static_assert(std::extent<typename Register::value_type>::value > 0, "Register array value is no fixed-size array");

		static_assert(std::is_volatile<typename Register::memory_type>::value, "Register array value is not volatile");

		static_assert(sizeof(Register) == sizeof(typename Register::value_type), "Register array contains superfluous fields");

		static_assert(alignof(Register) <= sizeof(typename Register::value_type), "Register array has weird alignment requirements");

		return true;
	};

	template<typename Register>
	constexpr bool is_platform_register_valid()
	{
		static_assert(
			std::is_base_of<
				ense::PlatformRegister<typename Register::bits_type, Register, typename Register::memory_type>,
				Register>::value,
			"Register must inherit PlatformRegister<_, Register, _> somewhere");

		static_assert(std::is_standard_layout<Register>::value, "Register is not standard-layout");

		static_assert(std::is_volatile<typename Register::memory_type>::value, "Register value is not volatile");

		static_assert(sizeof(Register) == sizeof(typename Register::value_type), "Register contains superfluous fields");

		static_assert(alignof(Register) <= sizeof(typename Register::value_type), "Register has weird alignment requirements");

		return true;
	};

}

template<typename Register>
using linker_placed_array = typename std::conditional<traits::is_platform_array_valid<Register>(), Register, void>::type;

template<typename Register>
using linker_placed_register = typename std::conditional<traits::is_platform_register_valid<Register>(), Register, void>::type;

}

#endif /* INCLUDE_HW_PLATFORM__REGISTER__HPP_ */
