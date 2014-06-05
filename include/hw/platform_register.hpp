#ifndef INCLUDE_HW_PLATFORM__REGISTER__HPP_EE91900D64381C0D
#define INCLUDE_HW_PLATFORM__REGISTER__HPP_EE91900D64381C0D

#include <stdint.h>
#include <type_traits>
#include <tuple>

#include <detail/bit.hpp>
#include <detail/array_wrapper.hpp>
#include <bitmask.hpp>
#include <mpl/all.hpp>
#include <mpl/min.hpp>

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
			static_assert(First <= Last && Last - First <= width && (First / width) == (Last / width), "");

			static constexpr uint32_t leading = First % width;
			static constexpr uint32_t trailing = width - 1 - Last % width;
			const Derived* self = static_cast<const Derived*>(this);
			return (self->word(First / width) << trailing) >> (leading + trailing);
		}

		word_type bits(uint32_t first, uint32_t last) const
		{
			uint32_t leading = first % width;
			uint32_t trailing = width - 1 - last % width;
			const Derived* self = static_cast<const Derived*>(this);
			return (self->word(first / width) << trailing) >> (leading + trailing);
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
			static constexpr uint32_t leading = First % width;
			static constexpr uint32_t trailing = width - 1 - Last % width;
			static constexpr auto max = ~typename WritablePlatformRegisterArray::word_type(0);
			static constexpr auto mask = (max >> (leading + trailing)) << leading;
			Derived* self = static_cast<Derived*>(this);
			return self->word(First / width, (self->word(First / width) & ~mask) | ((val << leading) & mask));
		}

		Derived& bits(uint32_t first, uint32_t last, typename WritablePlatformRegisterArray::word_type val)
		{
			static constexpr uint32_t width = WritablePlatformRegisterArray::width;
			uint32_t leading = first % width;
			uint32_t trailing = width - 1 - last % width;
			static constexpr auto max = ~typename WritablePlatformRegisterArray::word_type(0);
			auto mask = (max >> (leading + trailing)) << leading;
			Derived* self = static_cast<Derived*>(this);
			return self->word(first / width, (self->word(first / width) & ~mask) | ((val << leading) & mask));
		}

		Derived& word(uint32_t i, typename WritablePlatformRegisterArray::word_type val)
		{
			this->_value[i] = val;
			return static_cast<Derived&>(*this);
		}
};



template<class Derived, typename Value>
class PlatformRegisterPlainCommon {
	public:
		typename std::remove_cv<Value>::type value() const
		{
			return static_cast<const Derived*>(this)->word(0);
		}
};

template<typename Derived, typename Value>
class PlatformRegisterPlain : public PlatformRegisterArray<Derived, Value[1]>, public PlatformRegisterPlainCommon<Derived, Value> {
};

template<typename Derived, typename Value>
class WritablePlatformRegisterPlain : public WritablePlatformRegisterArray<Derived, Value[1]>, public PlatformRegisterPlainCommon<Derived, Value> {
	public:
		using PlatformRegisterPlainCommon<Derived, Value>::value;

		Derived& value(typename WritablePlatformRegisterPlain::word_type val)
		{
			return static_cast<Derived*>(this)->word(0, val);
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
class PlatformRegisterBitsCommon {
	public:
		typedef typename std::remove_cv<Bits>::type bits_type;

	public:
		template<typename... Args>
		bool has_any(Args... args) const
		{
			static_assert(mpl::all(std::is_same<Args, bits_type>::value...), "");

			auto mask = bitmask::bitmask(args...);
			return (static_cast<const Derived*>(this)->value() & mask) != 0;
		}

		template<typename... Args>
		bool has_all(Args... args) const
		{
			static_assert(mpl::all(std::is_same<Args, bits_type>::value...), "");

			auto mask = bitmask::bitmask(args...);
			return (static_cast<const Derived*>(this)->value() & mask) == mask;
		}
};

template<typename Derived, typename Bits>
class PlatformRegisterBits : public PlatformRegisterPlain<Derived, typename storage_type<Bits>::type>, public PlatformRegisterBitsCommon<Derived, Bits> {
};

template<typename Derived, typename Bits>
class WritablePlatformRegisterBits : public WritablePlatformRegisterPlain<Derived, typename storage_type<Bits>::type>, public PlatformRegisterBitsCommon<Derived, Bits> {
	public:
		template<typename... Args>
		Derived& set(Args... args)
		{
			static_assert(mpl::all(std::is_same<Args, typename WritablePlatformRegisterBits::bits_type>::value...), "");

			Derived* self = static_cast<Derived*>(this);
			return self->value(self->value() | bitmask::bitmask(args...));
		}

		template<typename... Args>
		Derived& clear(Args... args)
		{
			static_assert(mpl::all(std::is_same<Args, typename WritablePlatformRegisterBits::bits_type>::value...), "");

			Derived* self = static_cast<Derived*>(this);
			return self->value(self->value() & ~bitmask::bitmask(args...));
		}

		template<typename... Args>
		Derived& toggle(Args... args)
		{
			static_assert(mpl::all(std::is_same<Args, typename WritablePlatformRegisterBits::bits_type>::value...), "");

			Derived* self = static_cast<Derived*>(this);
			return self->value(self->value() ^ bitmask::bitmask(args...));
		}
};

}

template<typename Derived, typename Value>
class PlatformRegister :
	public std::conditional<
		std::is_enum<Value>::value,
		detail::PlatformRegisterBits<Derived, Value>,
		typename std::conditional<
			std::is_array<Value>::value,
			detail::PlatformRegisterArray<Derived, Value>,
			detail::PlatformRegisterPlain<Derived, Value>>::type>::type {
};

template<typename Derived, typename Value>
class WritablePlatformRegister :
	public std::conditional<
		std::is_enum<Value>::value,
		detail::WritablePlatformRegisterBits<Derived, Value>,
		typename std::conditional<
			std::is_array<Value>::value,
			detail::WritablePlatformRegisterArray<Derived, Value>,
			detail::WritablePlatformRegisterPlain<Derived, Value>>::type>::type {
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
	}

	template<typename Register>
	constexpr bool is_platform_register_valid()
	{
		static_assert(std::is_standard_layout<Register>::value, "Register is not standard-layout");

		static_assert(std::is_volatile<typename Register::memory_type>::value, "Register value is not volatile");

		static_assert(sizeof(Register) == sizeof(typename Register::value_type), "Register contains superfluous fields");

		return true;
	}

	template<typename Array, typename FieldDesc>
	constexpr bool assert_platform_array_type()
	{
		static_assert(std::rank<Array>::value == 1, "type must be one-dimensional");

		static_assert(std::is_base_of<detail::bit::has_element_offsets, FieldDesc>::value
				|| FieldDesc::range == std::extent<Array>::value * FieldDesc::width, "type does not fit range");

		return true;
	}

}

template<typename Register>
using linker_placed_array = typename std::conditional<traits::is_platform_array_valid<Register>(), Register, void>::type;

template<typename Register>
using linker_placed_register = typename std::conditional<traits::is_platform_register_valid<Register>(), Register, void>::type;

}

#endif /* INCLUDE_HW_PLATFORM__REGISTER__HPP_EE91900D64381C0D */
