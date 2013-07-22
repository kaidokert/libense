#ifndef INCLUDE_HW_STM32F4_GPIO__HPP_
#define INCLUDE_HW_STM32F4_GPIO__HPP_

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
class GPIO_MODER : public ConfigurationRegister<void, Config, GPIO_MODER> {
	public:
		REGISTER_SINGULAR_ARRAY_RW(PortFunction[16], bit::range<0, 31>, bit::width<2>)
};

static_assert(traits::is_platform_register_valid<GPIO_MODER<>>(), "");


enum class PortOutputType : uint32_t {
	push_pull  = 0,
	open_drain = 1
};

template<bool Config = false>
class GPIO_OTYPER : public ConfigurationRegister<void, Config, GPIO_OTYPER> {
	public:
		REGISTER_SINGULAR_ARRAY_RW(PortOutputType[16], bit::range<0, 15>, bit::width<1>)
};

static_assert(traits::is_platform_register_valid<GPIO_OTYPER<>>(), "");


enum class PortSpeed : uint32_t {
	low    = 0,
	medium = 1,
	fast   = 2,
	high   = 3
};

template<bool Config = false>
class GPIO_OSPEEDR : public ConfigurationRegister<void, Config, GPIO_OSPEEDR> {
	public:
		REGISTER_SINGULAR_ARRAY_RW(PortSpeed[16], bit::range<0, 31>, bit::width<2>)
};

static_assert(traits::is_platform_register_valid<GPIO_OSPEEDR<>>(), "");


enum class PortPull : uint32_t {
	none = 0,
	up   = 1,
	down = 2
};

template<bool Config = false>
class GPIO_PUPDR : public ConfigurationRegister<void, Config, GPIO_PUPDR> {
	public:
		REGISTER_SINGULAR_ARRAY_RW(PortPull[16], bit::range<0, 31>, bit::width<2>)
};

static_assert(traits::is_platform_register_valid<GPIO_PUPDR<>>(), "");


template<bool Config = false>
class GPIO_IDR : public ConfigurationRegister<void, Config, GPIO_IDR> {
	public:
		REGISTER_SINGULAR_ARRAY_R(bool[16], bit::range<0, 15>, bit::width<1>)
		REGISTER_FIELD_R(uint16_t, raw, bit::range<0, 15>)
};

static_assert(traits::is_platform_register_valid<GPIO_IDR<>>(), "");


template<bool Config = false>
class GPIO_ODR : public ConfigurationRegister<void, Config, GPIO_ODR> {
	public:
		REGISTER_SINGULAR_ARRAY_RW(bool[16], bit::range<0, 15>, bit::width<1>)
		REGISTER_FIELD_RW(uint16_t, raw, bit::range<0, 15>)
};

static_assert(traits::is_platform_register_valid<GPIO_ODR<>>(), "");


template<bool Config = false>
class GPIO_BSSR : public ConfigurationRegister<void, Config, GPIO_BSSR> {
	public:
		REGISTER_ARRAY_W(bool[16], set, bit::range<0, 15>, bit::width<1>)
		REGISTER_ARRAY_W(bool[16], reset, bit::range<16, 31>, bit::width<1>)
};

static_assert(traits::is_platform_register_valid<GPIO_BSSR<>>(), "");




class GPIO {
	private:
		GPIO_MODER<> _mode;
		GPIO_OTYPER<> _otype;
		GPIO_OSPEEDR<> _ospeed;
		GPIO_PUPDR<> _pupd;
		GPIO_IDR<> _idr;
		GPIO_ODR<> _odr;
		GPIO_BSSR<> _bssr;
		// lock
		// afh
		// afl

	public:
		GPIO_MODER<>& mode() { return _mode; }

		GPIO_OTYPER<>& output_type() { return _otype; }

		GPIO_OSPEEDR<>& speed() { return _ospeed; }

		GPIO_PUPDR<>& pull_state() { return _pupd; }

		GPIO_IDR<>& input() { return _idr; }

		GPIO_ODR<>& output() { return _odr; }

		GPIO_BSSR<>& set_reset() { return _bssr; }
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

#endif /* INCLUDE_HW_STM32F4_GPIO__HPP_ */
