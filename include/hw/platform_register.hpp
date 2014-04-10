#ifndef INCLUDE_HW_PLATFORM__REGISTER__HPP_EE91900D64381C0D
#define INCLUDE_HW_PLATFORM__REGISTER__HPP_EE91900D64381C0D

#include <stdint.h>
#include <type_traits>
#include <tuple>

#include <detail/bit.hpp>
#include <detail/const_array_wrapper.hpp>
#include <detail/array_wrapper.hpp>
#include <bitmask.hpp>
#include <mpl/all.hpp>

namespace ense {

namespace detail {

template<typename Derived, typename Value>
class PlatformRegisterArray {
	static_assert(std::is_array<Value>::value, "");
	static_assert(std::rank<typename std::remove_cv<Value>::type>::value == 1, "");
	static_assert(std::is_same<typename std::remove_all_extents<typename std::remove_cv<Value>::type>::type, uint32_t>::value, "");

	public:
		typedef Value memory_type;
		typedef typename std::remove_cv<memory_type>::type value_type;
		typedef typename std::remove_all_extents<value_type>::type word_type;
		static constexpr uint32_t words = std::extent<Value>::value;

	protected:
		static constexpr uint32_t width = sizeof(word_type) * 8;

	protected:
		Value _value;

	public:
		template<uint32_t First, uint32_t Last>
		word_type bits() const
		{
			static_assert(std::is_base_of<PlatformRegisterArray, Derived>::value, "");
			static_assert(First <= Last && Last - First <= width && (First / width) == (Last / width), "");

			uint32_t leading = First % width;
			uint32_t trailing = width - 1 - Last % width;
			return (this->word(First / width) << trailing) >> (leading + trailing);
		}

		word_type word(uint32_t idx) const
		{
			return _value[idx];
		}
};

template<typename Derived, typename Value>
class WritablePlatformRegisterArray : public PlatformRegisterArray<Derived, Value> {
	public:
		using PlatformRegisterArray<Derived, Value>::word;
		using PlatformRegisterArray<Derived, Value>::bits;

		template<uint32_t First, uint32_t Last>
		Derived& bits(typename WritablePlatformRegisterArray::word_type val)
		{
			static constexpr uint32_t width = WritablePlatformRegisterArray::width;
			uint32_t leading = First % width;
			uint32_t trailing = width - 1 - Last % width;
			auto max = ~typename WritablePlatformRegisterArray::word_type(0);
			auto mask = (max >> (leading + trailing)) << leading;
			return this->word(First / width, (this->word(First / width) & ~mask) | ((val << leading) & mask));
		}

		Derived& word(uint32_t i, typename WritablePlatformRegisterArray::value_type val)
		{
			static_assert(std::is_base_of<WritablePlatformRegisterArray, Derived>::value, "");

			this->_value[i] = val;
			return static_cast<Derived&>(*this);
		}
};



template<typename Derived, typename Value>
class PlatformRegisterPlain : public PlatformRegisterArray<Derived, Value[1]> {
	static_assert(std::is_same<typename std::remove_cv<Value>::type, uint32_t>::value, "");

	public:
		typedef Value memory_type;
		typedef typename std::remove_cv<memory_type>::type value_type;

	public:
		value_type value() const
		{
			static_assert(std::is_base_of<PlatformRegisterPlain, Derived>::value, "");

			return this->word(0);
		}
};

template<typename Derived, typename Value>
class WritablePlatformRegisterPlain : public PlatformRegisterPlain<Derived, Value> {
	public:
		using PlatformRegisterPlain<Derived, Value>::value;
		using PlatformRegisterPlain<Derived, Value>::word;
		using PlatformRegisterPlain<Derived, Value>::bits;

		template<uint32_t First, uint32_t Last>
		Derived& bits(typename WritablePlatformRegisterPlain::word_type val)
		{
			static constexpr uint32_t width = WritablePlatformRegisterPlain::width;
			uint32_t leading = First % width;
			uint32_t trailing = width - 1 - Last % width;
			auto max = ~typename WritablePlatformRegisterPlain::word_type(0);
			auto mask = (max >> (leading + trailing)) << leading;
			return this->word(First / width, (this->word(First / width) & ~mask) | ((val << leading) & mask));
		}

		Derived& word(uint32_t i, typename WritablePlatformRegisterPlain::word_type val)
		{
			static_assert(std::is_base_of<WritablePlatformRegisterPlain, Derived>::value, "");

			this->_value[i] = val;
			return static_cast<Derived&>(*this);
		}

		Derived& value(typename WritablePlatformRegisterPlain::word_type val)
		{
			return this->word(0, val);
		}
};



template<class Enum>
struct storage_type {
	typedef typename std::conditional<
		std::is_volatile<Enum>::value,
		volatile typename std::underlying_type<Enum>::type,
		typename std::underlying_type<Enum>::type>::type type;
};



template<typename Derived, typename Bits>
class PlatformRegisterBits : public PlatformRegisterPlain<Derived, typename storage_type<Bits>::type> {
	static_assert(
		std::is_same<
			typename std::underlying_type<Bits>::type,
			typename PlatformRegisterBits::value_type>::value,
		"Bits is not a Value-enum");

	public:
		typedef typename std::remove_cv<Bits>::type bits_type;

	public:
		template<typename... Args>
		bool has_any(Args... args) const
		{
			static_assert(std::is_base_of<PlatformRegisterBits, Derived>::value, "");
			static_assert(::ense::mpl::all(std::is_same<Args, bits_type>::value...), "");

			auto mask = ::ense::detail::bitmask::bitmask(args...);
			return (static_cast<const Derived*>(this)->value() & mask) != 0;
		}

		template<typename... Args>
		bool has_all(Args... args) const
		{
			static_assert(std::is_base_of<PlatformRegisterBits, Derived>::value, "");
			static_assert(::ense::mpl::all(std::is_same<Args, bits_type>::value...), "");

			auto mask = ::ense::detail::bitmask::bitmask(args...);
			return (static_cast<const Derived*>(this)->value() & mask) == mask;
		}
};

template<typename Derived, typename Bits>
class WritablePlatformRegisterBits : public PlatformRegisterBits<Derived, Bits> {
	public:
		template<typename... Args>
		Derived& set(Args... args)
		{
			static_assert(std::is_base_of<WritablePlatformRegisterBits, Derived>::value, "");
			static_assert(::ense::mpl::all(std::is_same<Args, typename WritablePlatformRegisterBits::bits_type>::value...), "");

			Derived* self = static_cast<Derived*>(this);
			return self->value(self->value() | ::ense::detail::bitmask::bitmask(args...));
		}

		template<typename... Args>
		Derived& clear(Args... args)
		{
			static_assert(std::is_base_of<WritablePlatformRegisterBits, Derived>::value, "");
			static_assert(::ense::mpl::all(std::is_same<Args, typename WritablePlatformRegisterBits::bits_type>::value...), "");

			Derived* self = static_cast<Derived*>(this);
			return self->value(self->value() & ~::ense::detail::bitmask::bitmask(args...));
		}

		using PlatformRegisterBits<Derived, Bits>::value;
		using PlatformRegisterBits<Derived, Bits>::word;
		using PlatformRegisterBits<Derived, Bits>::bits;

		template<uint32_t First, uint32_t Last>
		Derived& bits(typename WritablePlatformRegisterBits::word_type val)
		{
			static constexpr uint32_t width = WritablePlatformRegisterBits::width;
			uint32_t leading = First % width;
			uint32_t trailing = width - 1 - Last % width;
			auto max = ~typename WritablePlatformRegisterBits::word_type(0);
			auto mask = (max >> (leading + trailing)) << leading;
			return this->word(First / width, (this->word(First / width) & ~mask) | ((val << leading) & mask));
		}

		Derived& word(uint32_t i, typename WritablePlatformRegisterBits::word_type val)
		{
			static_assert(std::is_base_of<WritablePlatformRegisterBits, Derived>::value, "");

			this->_value[i] = val;
			return static_cast<Derived&>(*this);
		}

		Derived& value(typename WritablePlatformRegisterBits::value_type val)
		{
			static_assert(std::is_base_of<WritablePlatformRegisterBits, Derived>::value, "");

			this->_value[0] = val;
			return static_cast<Derived&>(*this);
		}
};

}

template<typename Derived, typename Value>
class PlatformRegister :
	public std::conditional<
		std::is_enum<Value>::value,
		::ense::detail::PlatformRegisterBits<Derived, Value>,
		::ense::detail::PlatformRegisterPlain<Derived, Value>>::type {
};

template<typename Derived, typename Value>
class WritablePlatformRegister :
	public std::conditional<
		std::is_enum<Value>::value,
		::ense::detail::WritablePlatformRegisterBits<Derived, Value>,
		::ense::detail::WritablePlatformRegisterPlain<Derived, Value>>::type {
};



namespace traits {

	template<typename Register>
	constexpr bool is_platform_array_valid()
	{
		static_assert(std::is_standard_layout<Register>::value, "Register array is not standard-layout");

		static_assert(std::extent<typename Register::value_type>::value > 0, "Register array value is no fixed-size array");

		static_assert(std::is_volatile<typename Register::memory_type>::value, "Register array value is not volatile");

		static_assert(sizeof(Register) == sizeof(typename Register::value_type), "Register array contains superfluous fields");

		return true;
	};

	template<typename Register>
	constexpr bool is_platform_register_valid()
	{
		static_assert(std::is_standard_layout<Register>::value, "Register is not standard-layout");

		static_assert(std::is_volatile<typename Register::memory_type>::value, "Register value is not volatile");

		static_assert(sizeof(Register) == sizeof(typename Register::value_type), "Register contains superfluous fields");

		return true;
	};

}

template<typename Register>
using linker_placed_array = typename std::conditional<traits::is_platform_array_valid<Register>(), Register, void>::type;

template<typename Register>
using linker_placed_register = typename std::conditional<traits::is_platform_register_valid<Register>(), Register, void>::type;

}

#endif /* INCLUDE_HW_PLATFORM__REGISTER__HPP_EE91900D64381C0D */
