#ifndef INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A
#define INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A

#include <hw/config_register.hpp>
#include <hw/platform_register_macros.hpp>

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




class GPIO {
	private:
		GPIO_MODER<> _mode;
		GPIO_OTYPER<> _otype;
		GPIO_OSPEEDR<> _ospeed;
		GPIO_PUPDR<> _pupd;
		GPIO_IDR<> _idr;
		GPIO_ODR<> _odr;
		GPIO_BSSR<> _bssr;
		volatile uint32_t _lock;
		GPIO_AFR<> _afl;
		GPIO_AFR<> _afh;

	public:
		GPIO_MODER<>& mode() { return _mode; }

		GPIO_OTYPER<>& output_type() { return _otype; }

		GPIO_OSPEEDR<>& speed() { return _ospeed; }

		GPIO_PUPDR<>& pull_state() { return _pupd; }

		GPIO_IDR<>& input() { return _idr; }

		GPIO_ODR<>& output() { return _odr; }

		GPIO_BSSR<>& set_reset() { return _bssr; }

		GPIO& lock(uint16_t bits)
		{
			asm volatile (
				"str %1, %0\n"
				"str %2, %0\n"
				"str %1, %0\n"
				"ldr r2, %0"
				: "+m" (_lock) : "r"(0x10000 | bits), "r"(bits) : "r2", "memory");
			return *this;
		}

		uint16_t lock() const
		{
			return _lock;
		}

		GPIO_AFR<>& afl() { return _afl; }

		GPIO_AFR<>& afh() { return _afh; }
};

static_assert(std::is_standard_layout<GPIO>::value, "");

extern GPIO gpioA __attribute__((__weak__, __alias__(".GPIO_PORTA")));
extern GPIO gpioB __attribute__((__weak__, __alias__(".GPIO_PORTB")));
extern GPIO gpioC __attribute__((__weak__, __alias__(".GPIO_PORTC")));
extern GPIO gpioD __attribute__((__weak__, __alias__(".GPIO_PORTD")));
extern GPIO gpioE __attribute__((__weak__, __alias__(".GPIO_PORTE")));
extern GPIO gpioF __attribute__((__weak__, __alias__(".GPIO_PORTF")));
extern GPIO gpioG __attribute__((__weak__, __alias__(".GPIO_PORTG")));
extern GPIO gpioH __attribute__((__weak__, __alias__(".GPIO_PORTH")));
extern GPIO gpioI __attribute__((__weak__, __alias__(".GPIO_PORTI")));

}
}
}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A */
