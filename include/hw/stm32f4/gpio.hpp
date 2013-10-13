#ifndef INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A
#define INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A

#include <hw/config_struct.hpp>
#include <hw/platform_register_macros.hpp>
#include <hw/config_struct_macros.hpp>
#include <mpl/select_memfn.hpp>
#include <tuple>

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
};

static_assert(traits::is_platform_register_valid<IDR<>>(), "");


template<bool Config = false>
struct ODR : ConfigurationRegister<void, Config, ODR> {
	REGISTER_SINGULAR_ARRAY_RW(bool[16], detail::bit::range<0, 15>, detail::bit::width<1>)
	REGISTER_FIELD_RW(uint16_t, raw, detail::bit::range<0, 15>)

	ODR& operator=(const ODR&) = default;

	uint16_t operator=(uint16_t val)
	{
		raw(val);
		return val;
	}

	operator uint16_t() const { return raw(); }

#define OP_EQ(op) \
	uint16_t operator op ## =(uint16_t val) \
	{ \
		auto next = raw() op val; \
		raw(next); \
		return next; \
	}
	OP_EQ(+)
	OP_EQ(-)
	OP_EQ(*)
	OP_EQ(/)
	OP_EQ(%)
	OP_EQ(^)
	OP_EQ(&)
	OP_EQ(|)
	OP_EQ(>>)
	OP_EQ(<<)
#undef OP_EQ
};

static_assert(traits::is_platform_register_valid<ODR<>>(), "");


template<bool Config = false>
struct BSSR : ConfigurationRegister<void, Config, BSSR> {
	REGISTER_ARRAY_C(bool[16], set, detail::bit::range<0, 15>, detail::bit::width<1>)
	REGISTER_ARRAY_C(bool[16], reset, detail::bit::range<16, 31>, detail::bit::width<1>)
};

static_assert(traits::is_platform_register_valid<BSSR<>>(), "");


struct AF {
	const uint32_t af;

	constexpr explicit AF(uint32_t af) : af(af) {}

	explicit operator uint32_t() { return af; }
};

template<bool Config = false>
struct AFR : ConfigurationRegister<void, Config, AFR> {
	REGISTER_SINGULAR_ARRAY_RW(AF[8], detail::bit::range<0, 31>, detail::bit::width<4>)
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

template<typename Flight = void>
struct GPIO : ConfigurationStruct<GPIO, detail::layout, Flight> {
	template<typename>
	friend struct GPIO;

	public:
		typedef detail::layout struct_type;
	private:
		typedef GPIO this_type;
		typedef Flight flight_type;
		template<typename Next>
		using this_template = GPIO<Next>;

	public:
		STRUCT_SINGULAR_ARRAY_RW(mode, moder)
		STRUCT_SINGULAR_ARRAY_RW(output_type, otyper)
		STRUCT_SINGULAR_ARRAY_RW(output_speed, ospeedr)
		STRUCT_SINGULAR_ARRAY_RW(pull, pupdr)
		STRUCT_SINGULAR_ARRAY_R(input, idr)
		STRUCT_SINGULAR_ARRAY_RW(output, odr)
		STRUCT_ARRAY_C(set, bssr, set)
		STRUCT_ARRAY_C(reset, bssr, reset)
		STRUCT_SINGULAR_MULTIARRAY_RW(alternate_function, AFR<>, STRUCT_OFFSETOF(afrl), STRUCT_OFFSETOF(afrh))

		STRUCT_CONFIGURE_MANY(
			STRUCT_CONFIGURE_SINGLE(mode)
			STRUCT_CONFIGURE_SINGLE(output_type)
			STRUCT_CONFIGURE_SINGLE(output_speed)
			STRUCT_CONFIGURE_SINGLE(pull)
			STRUCT_CONFIGURE_SINGLE(alternate_function)
		)

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

		uint32_t lock() const { return this->lckr; }

		uint16_t in() const { return this->idr.raw(); }

		ODR<>& out() { return this->odr; }
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
