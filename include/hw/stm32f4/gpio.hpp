#ifndef INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A
#define INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A

#include <hw/config_struct.hpp>
#include <hw/platform_register_macros.hpp>
#include <hw/config_struct_macros.hpp>

namespace ense {
namespace platform {
namespace gpio {

enum class PortFunction : uint32_t {
	input     = 0,
	output    = 1,
	alternate = 2,
	analog    = 3
};

template<bool Config = false>
struct GPIO_MODER : ConfigurationRegister<void, Config, GPIO_MODER> {
	REGISTER_SINGULAR_ARRAY_RW(PortFunction[16], detail::bit::range<0, 31>, detail::bit::width<2>)
};

static_assert(traits::is_platform_register_valid<GPIO_MODER<>>(), "");


enum class PortOutputType : uint32_t {
	push_pull  = 0,
	open_drain = 1
};

template<bool Config = false>
struct GPIO_OTYPER : ConfigurationRegister<void, Config, GPIO_OTYPER> {
	REGISTER_SINGULAR_ARRAY_RW(PortOutputType[16], detail::bit::range<0, 15>, detail::bit::width<1>)
};

static_assert(traits::is_platform_register_valid<GPIO_OTYPER<>>(), "");


enum class PortSpeed : uint32_t {
	low    = 0,
	medium = 1,
	fast   = 2,
	high   = 3
};

template<bool Config = false>
struct GPIO_OSPEEDR : ConfigurationRegister<void, Config, GPIO_OSPEEDR> {
	REGISTER_SINGULAR_ARRAY_RW(PortSpeed[16], detail::bit::range<0, 31>, detail::bit::width<2>)
};

static_assert(traits::is_platform_register_valid<GPIO_OSPEEDR<>>(), "");


enum class PortPull : uint32_t {
	none = 0,
	up   = 1,
	down = 2
};

template<bool Config = false>
struct GPIO_PUPDR : ConfigurationRegister<void, Config, GPIO_PUPDR> {
	REGISTER_SINGULAR_ARRAY_RW(PortPull[16], detail::bit::range<0, 31>, detail::bit::width<2>)
};

static_assert(traits::is_platform_register_valid<GPIO_PUPDR<>>(), "");


template<bool Config = false>
struct GPIO_IDR : ConfigurationRegister<void, Config, GPIO_IDR> {
	REGISTER_SINGULAR_ARRAY_R(bool[16], detail::bit::range<0, 15>, detail::bit::width<1>)
	REGISTER_FIELD_R(uint16_t, raw, detail::bit::range<0, 15>)

	operator uint16_t() const
	{
		return raw();
	}
};

static_assert(traits::is_platform_register_valid<GPIO_IDR<>>(), "");


template<bool Config = false>
struct GPIO_ODR : ConfigurationRegister<void, Config, GPIO_ODR> {
	REGISTER_SINGULAR_ARRAY_RW(bool[16], detail::bit::range<0, 15>, detail::bit::width<1>)
	REGISTER_FIELD_RW(uint16_t, raw, detail::bit::range<0, 15>)

	uint16_t operator=(uint16_t value)
	{
		raw(value);
		return value;
	}

	operator uint16_t() const
	{
		return raw();
	}
};

static_assert(traits::is_platform_register_valid<GPIO_ODR<>>(), "");


template<bool Config = false>
struct GPIO_BSSR : ConfigurationRegister<void, Config, GPIO_BSSR> {
	REGISTER_ARRAY_C(bool[16], set, detail::bit::range<0, 15>, detail::bit::width<1>)
	REGISTER_ARRAY_C(bool[16], reset, detail::bit::range<16, 31>, detail::bit::width<1>)
};

static_assert(traits::is_platform_register_valid<GPIO_BSSR<>>(), "");


template<bool Config = false>
struct GPIO_AFR : ConfigurationRegister<void, Config, GPIO_AFR> {
	REGISTER_SINGULAR_ARRAY_RW(uint8_t[8], detail::bit::range<0, 31>, detail::bit::width<4>)
};

static_assert(traits::is_platform_register_valid<GPIO_AFR<>>(), "");




namespace detail {

	struct gpio_layout {
		GPIO_MODER<> moder;
		GPIO_OTYPER<> otyper;
		GPIO_OSPEEDR<> ospeedr;
		GPIO_PUPDR<> pupdr;
		GPIO_IDR<> idr;
		GPIO_ODR<> odr;
		GPIO_BSSR<> bssr;
		volatile uint32_t lckr;
		GPIO_AFR<> afrl;
		GPIO_AFR<> afrh;
	};

}

template<typename Flight = void>
struct GPIO : ConfigurationStruct<GPIO, detail::gpio_layout, Flight> {
	template<typename>
	friend struct GPIO;

	public:
		typedef detail::gpio_layout struct_type;

	private:
		template<typename FlightType, uint32_t Offset>
		struct afr_next {
		   typedef typename ense::detail::extend_flight_type<ense::detail::ConfigurationStructFlightPart<Offset, GPIO_AFR<>::in_flight_type>, FlightType>::type type;
		};
		template<uint32_t Offset>
		struct afr_next<void, Offset> {
			typedef void type;
		};

		typedef typename std::conditional<
			std::is_same<Flight, void>::value,
			GPIO&,
			GPIO<typename afr_next<typename afr_next<Flight, STRUCT_OFFSETOF(afrl)>::type, STRUCT_OFFSETOF(afrh)>::type>
			>::type alternate_function_next_type;

		template<typename T>
		static alternate_function_next_type begin_apply_af(T& self)
		{
			return self.template extend<STRUCT_OFFSETOF(afrl)>(self.target()->afrl)
				.template extend<STRUCT_OFFSETOF(afrh)>(self.target()->afrh);
		}
		static alternate_function_next_type begin_apply_af(alternate_function_next_type& self)
		{
			return self;
		}

		template<uint32_t Idx, uint32_t Mask>
		alternate_function_next_type apply_af(uint32_t fn, std::integral_constant<uint32_t, Mask>)
		{
			if (Mask & 1) {
				return alternate_function(Idx, fn).template apply_af<Idx + 1>(fn, std::integral_constant<uint32_t, (Mask >> 1)>());
			} else {
				return apply_af<Idx + 1>(fn, std::integral_constant<uint32_t, (Mask >> 1)>());
			}
		}
		template<uint32_t Idx>
		alternate_function_next_type apply_af(uint32_t, std::integral_constant<uint32_t, 0>)
		{
			return begin_apply_af(*this);
		}

	public:
		STRUCT_SINGULAR_ARRAY_RW(mode, uint32_t, moder)
		STRUCT_SINGULAR_ARRAY_RW(output_type, uint32_t, otyper)
		STRUCT_SINGULAR_ARRAY_RW(output_speed, uint32_t, ospeedr)
		STRUCT_SINGULAR_ARRAY_RW(pull, uint32_t, pupdr)
		STRUCT_SINGULAR_ARRAY_R(input, uint32_t, idr)
		STRUCT_SINGULAR_ARRAY_RW(output, uint32_t, odr)
		STRUCT_ARRAY_C(set, uint32_t, bssr, set)
		STRUCT_ARRAY_C(reset, uint32_t, bssr, reset)

		auto alternate_function(uint32_t idx, uint32_t fn)
			-> alternate_function_next_type
		{
			alternate_function_next_type extended = begin_apply_af(*this);
			if (idx > 7) {
				STRUCT_EXTRACT(extended, afrh).set(idx - 8, fn);
			} else {
				STRUCT_EXTRACT(extended, afrl).set(idx, fn);
			}
			return extended;
		}

		template<uint32_t Mask>
		auto alternate_function_mask(uint32_t fn)
			-> alternate_function_next_type
		{
			static_assert(Mask <= 0xFFFF, "Mask invalid");
			return apply_af<0>(fn, std::integral_constant<uint32_t, Mask>());
		}

		template<uint32_t Bound1, uint32_t Bound2>
		auto alternate_function_range(uint32_t fn)
			-> alternate_function_next_type
		{
			typedef ense::detail::bit::expand<ense::detail::bit::range<Bound1, Bound2>> range;
			static_assert(range::end < 16, "Index out or range");
			return alternate_function_mask<range::field_mask>(fn);
		}

		template<typename = void>
		GPIO& lock(uint16_t bits)
		{
			static_assert(std::is_same<Flight, void>::value, "Cannot lock configuration flights");
			asm volatile (
				"str %1, %0\n"
				"str %2, %0\n"
				"str %1, %0\n"
				"ldr r2, %0"
				: "+m" (this->lckr) : "r"(0x10000 | bits), "r"(bits) : "r2", "memory");
			return *this;
		}

		uint32_t lock()
		{
			return this->lckr;
		}
};

extern linker_placed_struct<GPIO> gpioA __attribute__((__weak__, __alias__(".GPIO_PORTA")));
extern linker_placed_struct<GPIO> gpioB __attribute__((__weak__, __alias__(".GPIO_PORTB")));
extern linker_placed_struct<GPIO> gpioC __attribute__((__weak__, __alias__(".GPIO_PORTC")));
extern linker_placed_struct<GPIO> gpioD __attribute__((__weak__, __alias__(".GPIO_PORTD")));
extern linker_placed_struct<GPIO> gpioE __attribute__((__weak__, __alias__(".GPIO_PORTE")));
extern linker_placed_struct<GPIO> gpioF __attribute__((__weak__, __alias__(".GPIO_PORTF")));
extern linker_placed_struct<GPIO> gpioG __attribute__((__weak__, __alias__(".GPIO_PORTG")));
extern linker_placed_struct<GPIO> gpioH __attribute__((__weak__, __alias__(".GPIO_PORTH")));
extern linker_placed_struct<GPIO> gpioI __attribute__((__weak__, __alias__(".GPIO_PORTI")));

}
}
}

#include <hw/platform_register_macros_clear.hpp>
#include <hw/config_struct_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A */
