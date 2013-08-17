#ifndef INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A
#define INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A

#include <hw/config_struct.hpp>
#include <hw/platform_register_macros.hpp>
#include <hw/config_struct_macros.hpp>

namespace ense {
namespace platform {
namespace gpio {

enum class Function : uint32_t {
	input     = 0,
	output    = 1,
	alternate = 2,
	analog    = 3
};

template<bool Config = false>
struct MODER : ConfigurationRegister<void, Config, MODER> {
	REGISTER_SINGULAR_ARRAY_RW(Function[16], detail::bit::range<0, 31>, detail::bit::width<2>)
};

static_assert(traits::is_platform_register_valid<MODER<>>(), "");


enum class OutputType : uint32_t {
	push_pull  = 0,
	open_drain = 1
};

template<bool Config = false>
struct OTYPER : ConfigurationRegister<void, Config, OTYPER> {
	REGISTER_SINGULAR_ARRAY_RW(OutputType[16], detail::bit::range<0, 15>, detail::bit::width<1>)
};

static_assert(traits::is_platform_register_valid<OTYPER<>>(), "");


enum class Speed : uint32_t {
	low    = 0,
	medium = 1,
	fast   = 2,
	high   = 3
};

template<bool Config = false>
struct OSPEEDR : ConfigurationRegister<void, Config, OSPEEDR> {
	REGISTER_SINGULAR_ARRAY_RW(Speed[16], detail::bit::range<0, 31>, detail::bit::width<2>)
};

static_assert(traits::is_platform_register_valid<OSPEEDR<>>(), "");


enum class Pull : uint32_t {
	none = 0,
	up   = 1,
	down = 2
};

template<bool Config = false>
struct PUPDR : ConfigurationRegister<void, Config, PUPDR> {
	REGISTER_SINGULAR_ARRAY_RW(Pull[16], detail::bit::range<0, 31>, detail::bit::width<2>)
};

static_assert(traits::is_platform_register_valid<PUPDR<>>(), "");


template<bool Config = false>
struct IDR : ConfigurationRegister<void, Config, IDR> {
	REGISTER_SINGULAR_ARRAY_R(bool[16], detail::bit::range<0, 15>, detail::bit::width<1>)
	REGISTER_FIELD_R(uint16_t, raw, detail::bit::range<0, 15>)

	operator uint16_t() const
	{
		return raw();
	}
};

static_assert(traits::is_platform_register_valid<IDR<>>(), "");


template<bool Config = false>
struct ODR : ConfigurationRegister<void, Config, ODR> {
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

static_assert(traits::is_platform_register_valid<ODR<>>(), "");


template<bool Config = false>
struct BSSR : ConfigurationRegister<void, Config, BSSR> {
	REGISTER_ARRAY_C(bool[16], set, detail::bit::range<0, 15>, detail::bit::width<1>)
	REGISTER_ARRAY_C(bool[16], reset, detail::bit::range<16, 31>, detail::bit::width<1>)
};

static_assert(traits::is_platform_register_valid<BSSR<>>(), "");


template<bool Config = false>
struct AFR : ConfigurationRegister<void, Config, AFR> {
	REGISTER_SINGULAR_ARRAY_RW(uint8_t[8], detail::bit::range<0, 31>, detail::bit::width<4>)
};

static_assert(traits::is_platform_register_valid<AFR<>>(), "");




namespace detail {

	struct layout {
		MODER<> moder;
		OTYPER<> otyper;
		OSPEEDR<> ospeedr;
		PUPDR<> pupdr;
		IDR<> idr;
		ODR<> odr;
		BSSR<> bssr;
		volatile uint32_t lckr;
		AFR<> afrl;
		AFR<> afrh;
	};

}

struct AF {
	const uint32_t af;

	constexpr explicit AF(uint32_t af) : af(af) {}
};

template<typename Flight = void>
struct GPIO : ConfigurationStruct<GPIO, detail::layout, Flight> {
	template<typename>
	friend struct GPIO;

	public:
		typedef detail::layout struct_type;

	private:
		template<typename FlightType, uint32_t Offset>
		struct afr_next {
		   typedef typename ense::detail::extend_flight_type<ense::detail::ConfigurationStructFlightPart<Offset, AFR<>::in_flight_type>, FlightType>::type type;
		};
		template<uint32_t Offset>
		struct afr_next<void, Offset> {
			typedef void type;
		};

		static constexpr bool is_config = !std::is_same<Flight, void>::value;
		typedef typename std::conditional<
			!is_config,
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
		alternate_function_next_type apply_af(AF fn, std::integral_constant<uint32_t, Mask>)
		{
			if (Mask & 1) {
				return alternate_function(Idx, fn).template apply_af<Idx + 1>(fn, std::integral_constant<uint32_t, (Mask >> 1)>());
			} else {
				return apply_af<Idx + 1>(fn, std::integral_constant<uint32_t, (Mask >> 1)>());
			}
		}
		template<uint32_t Idx>
		alternate_function_next_type apply_af(AF, std::integral_constant<uint32_t, 0>)
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

		auto alternate_function(uint32_t idx, AF fn)
			-> alternate_function_next_type
		{
			alternate_function_next_type extended = begin_apply_af(*this);
			if (idx > 7) {
				STRUCT_EXTRACT(extended, afrh).set(idx - 8, fn.af);
			} else {
				STRUCT_EXTRACT(extended, afrl).set(idx, fn.af);
			}
			return extended;
		}

		auto alternate_function_mask(uint32_t mask, AF fn)
			-> alternate_function_next_type
		{
			alternate_function_next_type extended = begin_apply_af(*this);
			for (uint32_t i = 0; i < 16; i++) {
				if (mask & (1 << i)) {
					if (is_config) {
						extended = extended.alternate_function(i, fn);
					} else {
						alternate_function(i, fn);
					}
				}
			}
			return extended;
		}

		template<uint32_t Mask>
		auto alternate_function_mask(AF fn)
			-> alternate_function_next_type
		{
			static_assert(Mask <= 0xFFFF, "Mask invalid");
			return apply_af<0>(fn, std::integral_constant<uint32_t, Mask>());
		}

		template<uint32_t Bound1, uint32_t Bound2>
		auto alternate_function_range(AF fn)
			-> alternate_function_next_type
		{
			typedef ense::detail::bit::expand<ense::detail::bit::range<Bound1, Bound2>> range;
			static_assert(range::end < 16, "Index out or range");
			return alternate_function_mask<range::field_mask>(fn);
		}

#define CONFIG_ONE(target) \
		auto configure(uint32_t pin, typename ense::mpl::nth_arg<1, decltype(ense::detail::select_memfn2(&GPIO::target))>::type arg) \
			-> decltype(this->target(pin, arg)) \
		{ \
			return this->target(pin, arg); \
		} \
		template<uint32_t Mask> \
		auto configure_mask(typename ense::mpl::nth_arg<1, decltype(ense::detail::select_memfn2(&GPIO::target))>::type arg) \
			-> decltype(this->target ## _mask<Mask>(arg)) \
		{ \
			return this->target ## _mask<Mask>(arg); \
		} \
		template<uint32_t Bound1, uint32_t Bound2> \
		auto configure_range(typename ense::mpl::nth_arg<1, decltype(ense::detail::select_memfn2(&GPIO::target))>::type arg) \
			-> decltype(this->target ## _range<Bound1, Bound2>(arg)) \
		{ \
			return this->target ## _range<Bound1, Bound2>(arg); \
		}
		CONFIG_ONE(mode)
		CONFIG_ONE(output_type)
		CONFIG_ONE(output_speed)
		CONFIG_ONE(pull)
		CONFIG_ONE(alternate_function)
#undef CONFIG_ONE

		template<typename First, typename... Rest>
		auto configure(uint32_t pin, First first, Rest... rest)
			-> decltype(this->configure(pin, first).configure(pin, rest...))
		{
			return this->configure(pin, first).configure(pin, rest...);
		}
		template<uint32_t Mask, typename First, typename... Rest>
		auto configure_mask(First first, Rest... rest)
			-> decltype(this->configure_mask<Mask>(first).template configure_mask<Mask>(rest...))
		{
			return this->configure_mask<Mask>(first).template configure_mask<Mask>(rest...);
		}
		template<uint32_t Bound1, uint32_t Bound2, typename First, typename... Rest>
		auto configure_range(First first, Rest... rest)
			-> decltype(this->configure_range<Bound1, Bound2>(first).template configure_range<Bound1, Bound2>(rest...))
		{
			return this->configure_range<Bound1, Bound2>(first).template configure_range<Bound1, Bound2>(rest...);
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

extern linker_placed_struct<GPIO> gpioA [[gnu::weak, gnu::alias(".GPIO_PORTA")]];
extern linker_placed_struct<GPIO> gpioB [[gnu::weak, gnu::alias(".GPIO_PORTB")]];
extern linker_placed_struct<GPIO> gpioC [[gnu::weak, gnu::alias(".GPIO_PORTC")]];
extern linker_placed_struct<GPIO> gpioD [[gnu::weak, gnu::alias(".GPIO_PORTD")]];
extern linker_placed_struct<GPIO> gpioE [[gnu::weak, gnu::alias(".GPIO_PORTE")]];
extern linker_placed_struct<GPIO> gpioF [[gnu::weak, gnu::alias(".GPIO_PORTF")]];
extern linker_placed_struct<GPIO> gpioG [[gnu::weak, gnu::alias(".GPIO_PORTG")]];
extern linker_placed_struct<GPIO> gpioH [[gnu::weak, gnu::alias(".GPIO_PORTH")]];
extern linker_placed_struct<GPIO> gpioI [[gnu::weak, gnu::alias(".GPIO_PORTI")]];

}
}
}

#include <hw/platform_register_macros_clear.hpp>
#include <hw/config_struct_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A */
