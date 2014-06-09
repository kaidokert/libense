#ifndef INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A
#define INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A

#include <hw/rcc.hpp>
#include <hw/config_struct.hpp>
#include <hw/platform_register_macros.hpp>
#include <hw/config_struct_macros.hpp>
#include <mpl/select_memfn.hpp>

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
	REGISTER_SINGULAR_ARRAY_RW(Function[16], range<0, 31>, width<2>)
};

static_assert(traits::is_platform_register_valid<MODER<>>(), "");


enum class OutputType : uint32_t {
	push_pull  = 0,
	open_drain = 1
};

template<bool Config = false>
struct OTYPER : ConfigurationRegister<void, Config, OTYPER> {
	REGISTER_SINGULAR_ARRAY_RW(OutputType[16], range<0, 15>, width<1>)
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
	REGISTER_SINGULAR_ARRAY_RW(Speed[16], range<0, 31>, width<2>)
};

static_assert(traits::is_platform_register_valid<OSPEEDR<>>(), "");


enum class Pull : uint32_t {
	none = 0,
	up   = 1,
	down = 2
};

template<bool Config = false>
struct PUPDR : ConfigurationRegister<void, Config, PUPDR> {
	REGISTER_SINGULAR_ARRAY_RW(Pull[16], range<0, 31>, width<2>)
};

static_assert(traits::is_platform_register_valid<PUPDR<>>(), "");


template<bool Config = false>
struct IDR : ConfigurationRegister<void, Config, IDR> {
	REGISTER_SINGULAR_ARRAY_R(bool[16], range<0, 15>, width<1>)
	REGISTER_FIELD_R(uint16_t, raw, range<0, 15>)
};

static_assert(traits::is_platform_register_valid<IDR<>>(), "");


template<bool Config = false>
struct ODR : ConfigurationRegister<void, Config, ODR> {
	REGISTER_SINGULAR_ARRAY_RW(bool[16], range<0, 15>, width<1>)
	REGISTER_FIELD_RW(uint16_t, raw, range<0, 15>)

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
struct BSRR : ConfigurationRegister<void, Config, BSRR> {
	static constexpr bool is_write_only = true;

	REGISTER_ARRAY_C(bool[16], set, range<0, 15>, width<1>)
	REGISTER_ARRAY_C(bool[16], reset, range<16, 31>, width<1>)
};

static_assert(traits::is_platform_register_valid<BSRR<>>(), "");


struct AF {
	const uint32_t af;

	constexpr explicit AF(uint32_t af) : af(af) {}

	explicit operator uint32_t() { return af; }
};

template<bool Config = false>
struct AFR : ConfigurationRegister<uint32_t[2], Config, AFR> {
	REGISTER_SINGULAR_ARRAY_RW(AF[16], range<0, 63>, width<4>)
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
		BSRR<> bsrr;
		volatile uint32_t lckr;
		AFR<> afr;
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

	public:
		STRUCT_SINGULAR_ARRAY_RW(mode, moder)
		STRUCT_SINGULAR_ARRAY_RW(output_type, otyper)
		STRUCT_SINGULAR_ARRAY_RW(output_speed, ospeedr)
		STRUCT_SINGULAR_ARRAY_RW(pull, pupdr)
		STRUCT_SINGULAR_ARRAY_R(input, idr)
		STRUCT_SINGULAR_ARRAY_RW(output, odr)
		STRUCT_ARRAY_C(set, bsrr, set)
		STRUCT_ARRAY_C(reset, bsrr, reset)
		STRUCT_SINGULAR_ARRAY_RW(alternate_function, afr)

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

template<typename PeripheralType, PeripheralType Bit>
struct GPIOInst : linker_placed_struct<GPIO> {
	struct rcc_info {
		typedef PeripheralType type;
		static constexpr PeripheralType bit = Bit;
	};
};

static GPIOInst<ense::platform::rcc::AHB1Peripheral, ense::platform::rcc::AHB1Peripheral::gpioA> gpioA [[gnu::weakref(".GPIO_PORTA")]];
static GPIOInst<ense::platform::rcc::AHB1Peripheral, ense::platform::rcc::AHB1Peripheral::gpioB> gpioB [[gnu::weakref(".GPIO_PORTB")]];
static GPIOInst<ense::platform::rcc::AHB1Peripheral, ense::platform::rcc::AHB1Peripheral::gpioC> gpioC [[gnu::weakref(".GPIO_PORTC")]];
static GPIOInst<ense::platform::rcc::AHB1Peripheral, ense::platform::rcc::AHB1Peripheral::gpioD> gpioD [[gnu::weakref(".GPIO_PORTD")]];
static GPIOInst<ense::platform::rcc::AHB1Peripheral, ense::platform::rcc::AHB1Peripheral::gpioE> gpioE [[gnu::weakref(".GPIO_PORTE")]];
static GPIOInst<ense::platform::rcc::AHB1Peripheral, ense::platform::rcc::AHB1Peripheral::gpioF> gpioF [[gnu::weakref(".GPIO_PORTF")]];
static GPIOInst<ense::platform::rcc::AHB1Peripheral, ense::platform::rcc::AHB1Peripheral::gpioG> gpioG [[gnu::weakref(".GPIO_PORTG")]];
static GPIOInst<ense::platform::rcc::AHB1Peripheral, ense::platform::rcc::AHB1Peripheral::gpioH> gpioH [[gnu::weakref(".GPIO_PORTH")]];
static GPIOInst<ense::platform::rcc::AHB1Peripheral, ense::platform::rcc::AHB1Peripheral::gpioI> gpioI [[gnu::weakref(".GPIO_PORTI")]];

}
}
}

#include <hw/platform_register_macros_clear.hpp>
#include <hw/config_struct_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A */
