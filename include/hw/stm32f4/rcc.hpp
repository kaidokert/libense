#ifndef INCLUDE_HW_STM32F4_RCC__HPP_645C98828FBCC4F0
#define INCLUDE_HW_STM32F4_RCC__HPP_645C98828FBCC4F0

#include <stdint.h>

#include <hw/config_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {
namespace platform {
namespace rcc {

enum class ClockControlFlags : uint32_t {
	plli2s_ready      = 1 << 27,
	plli2s_on         = 1 << 26,
	pll_ready         = 1 << 25,
	pll_on            = 1 << 24,
	clock_security_on = 1 << 19,
	hse_bypass        = 1 << 18,
	hse_ready         = 1 << 17,
	hse_on            = 1 << 16,
	hsi_ready         = 1 << 1,
	hsi_on            = 1 << 0
};

template<bool Config = false>
struct ClockControl : ConfigurationRegister<ClockControlFlags, Config, ClockControl> {
	REGISTER_BIT_R(plli2s_ready)
	REGISTER_BIT_RW(plli2s_on)
	REGISTER_BIT_R(pll_ready)
	REGISTER_BIT_RW(pll_on)
	REGISTER_BIT_RW(clock_security_on)
	REGISTER_BIT_RW(hse_bypass)
	REGISTER_BIT_R(hse_ready)
	REGISTER_BIT_RW(hse_on)
	REGISTER_INT_R(hsi_cal, range<15, 8>)
	REGISTER_INT_RW(hsi_trim, range<7, 3>)
	REGISTER_BIT_R(hsi_ready)
	REGISTER_BIT_RW(hsi_on)
};

static linker_placed_register<ClockControl<>> clock_control [[gnu::weakref(".RCC_CR")]];




enum class PLLSource : uint32_t {
	hsi = 0,
	hse = 1
};

enum class PLLDivider : uint32_t {
	div_2 = 0,
	div_4 = 1,
	div_6 = 2,
	div_8 = 3
};

template<bool Config = false>
struct PLLConfig : ConfigurationRegister<void, Config, PLLConfig> {
	REGISTER_INT_RW(q, range<27, 24>)
	REGISTER_FIELD_RW(PLLSource, pll_source, range<22, 22>)
	REGISTER_FIELD_RW(PLLDivider, p, range<17, 16>)
	REGISTER_INT_RW(n, range<14, 6>)
	REGISTER_INT_RW(m, range<5, 0>)
};

static linker_placed_register<PLLConfig<>> pll_config [[gnu::weakref(".RCC_PLLCFGR")]];




enum class I2SClockSource : uint32_t {
	plli2s   = 0,
	external = 1
};

enum class ClockOutSource : uint32_t {
	system_clock = 0,
	plli2s       = 1,
	hse          = 2,
	pll          = 3
};

enum class ClockOutDivider : uint32_t {
	div_1 = 0,
	div_2 = 4,
	div_3 = 5,
	div_4 = 6,
	div_5 = 7
};

enum class APBPrescaler : uint32_t {
	div_1  = 0,
	div_2  = 4,
	div_4  = 5,
	div_8  = 6,
	div_16 = 7
};

enum class AHBPrescaler : uint32_t {
	div_1   = 0,
	div_2   = 8,
	div_4   = 9,
	div_8   = 10,
	div_16  = 11,
	div_64  = 12,
	div_128 = 13,
	div_256 = 14,
	div_512 = 15
};

enum SystemClockSource : uint32_t {
	hsi = 0,
	hse = 1,
	pll = 2
};

template<bool Config = false>
struct ClockConfig : ConfigurationRegister<void, Config, ClockConfig> {
	REGISTER_FIELD_RW(ClockOutSource, mco2_source, range<31, 30>)
	REGISTER_FIELD_RW(ClockOutDivider, mco2_prescaler, range<29, 27>)
	REGISTER_FIELD_RW(ClockOutDivider, mco1_prescaler, range<26, 24>)
	REGISTER_FIELD_RW(I2SClockSource, i2s_source, range<23, 23>)
	REGISTER_FIELD_RW(ClockOutSource, mco1_source, range<22, 21>)
	REGISTER_INT_RW(rtc_prescaler, range<20, 16>)
	REGISTER_FIELD_RW(APBPrescaler, apb2_prescaler, range<15, 13>)
	REGISTER_FIELD_RW(APBPrescaler, apb1_prescaler, range<12, 10>)
	REGISTER_FIELD_RW(AHBPrescaler, ahb_prescaler, range<7, 4>)
	REGISTER_FIELD_RW(SystemClockSource, clock_source_status, range<3, 2>)
	REGISTER_FIELD_RW(SystemClockSource, clock_source_switch, range<1, 0>)
};

static linker_placed_register<ClockConfig<>> clock_config [[gnu::weakref(".RCC_CFGR")]];




enum class ClockInterruptFlags : uint32_t {
	clear_cssf          = 1 << 23,
	clear_plli2s_ready  = 1 << 21,
	clear_pll_ready     = 1 << 20,
	clear_hse_ready     = 1 << 19,
	clear_hsi_ready     = 1 << 18,
	clear_lse_ready     = 1 << 17,
	clear_lsi_ready     = 1 << 16,

	enable_plli2s_ready = 1 << 13,
	enable_pll_ready    = 1 << 12,
	enable_hse_ready    = 1 << 11,
	enable_hsi_ready    = 1 << 10,
	enable_lse_ready    = 1 << 9,
	enable_lsi_ready    = 1 << 8,
	enable_cssf         = 1 << 7,

	plli2s_ready        = 1 << 5,
	pll_ready           = 1 << 4,
	hse_ready           = 1 << 3,
	hsi_ready           = 1 << 2,
	lse_ready           = 1 << 1,
	lsi_ready           = 1 << 0
};

template<bool Config = false>
struct ClockInterrupt : ConfigurationRegister<ClockInterruptFlags, Config, ClockInterrupt> {
	REGISTER_BIT_C(clear_cssf)
	REGISTER_BIT_C(clear_plli2s_ready)
	REGISTER_BIT_C(clear_pll_ready)
	REGISTER_BIT_C(clear_hse_ready)
	REGISTER_BIT_C(clear_hsi_ready)
	REGISTER_BIT_C(clear_lse_ready)
	REGISTER_BIT_C(clear_lsi_ready)

	REGISTER_BIT_RW(enable_plli2s_ready)
	REGISTER_BIT_RW(enable_pll_ready)
	REGISTER_BIT_RW(enable_hse_ready)
	REGISTER_BIT_RW(enable_hsi_ready)
	REGISTER_BIT_RW(enable_lse_ready)
	REGISTER_BIT_RW(enable_lsi_ready)
	REGISTER_BIT_RW(enable_cssf)

	REGISTER_BIT_R(plli2s_ready)
	REGISTER_BIT_R(pll_ready)
	REGISTER_BIT_R(hse_ready)
	REGISTER_BIT_R(hsi_ready)
	REGISTER_BIT_R(lse_ready)
	REGISTER_BIT_R(lsi_ready)
};

static linker_placed_register<ClockInterrupt<>> clockinterrupt [[gnu::weakref(".RCC_CIR")]];




enum class AHB1Peripheral : uint32_t {
	usb_otg_hs_ulpi = 1U << 30,
	usb_otg_hs      = 1U << 29,
	eth_mac_ptp     = 1U << 28,
	eth_mac_rx      = 1U << 27,
	eth_mac_tx      = 1U << 26,
	eth_mac         = 1U << 25,
	dma2            = 1U << 22,
	dma1            = 1U << 21,
	ccm_data_ram    = 1U << 20,
	sram3           = 1U << 18,
	backup_sram     = 1U << 18,
	crc             = 1U << 12,
	gpioI           = 1U << 8,
	gpioH           = 1U << 7,
	gpioG           = 1U << 6,
	gpioF           = 1U << 5,
	gpioE           = 1U << 4,
	gpioD           = 1U << 3,
	gpioC           = 1U << 2,
	gpioB           = 1U << 1,
	gpioA           = 1U << 0
};

template<bool Config, template<bool> class Inner>
struct AHB1Clock : ConfigurationRegister<AHB1Peripheral, Config, Inner> {
	REGISTER_BIT_RW(usb_otg_hs_ulpi)
	REGISTER_BIT_RW(usb_otg_hs)
	REGISTER_BIT_RW(eth_mac_ptp)
	REGISTER_BIT_RW(eth_mac_rx)
	REGISTER_BIT_RW(eth_mac_tx)
	REGISTER_BIT_RW(eth_mac)
	REGISTER_BIT_RW(dma2)
	REGISTER_BIT_RW(dma1)
	REGISTER_BIT_RW(ccm_data_ram)
	REGISTER_BIT_RW(sram3)
	REGISTER_BIT_RW(backup_sram)
	REGISTER_BIT_RW(crc)
	REGISTER_BIT_RW(gpioI)
	REGISTER_BIT_RW(gpioH)
	REGISTER_BIT_RW(gpioG)
	REGISTER_BIT_RW(gpioF)
	REGISTER_BIT_RW(gpioE)
	REGISTER_BIT_RW(gpioD)
	REGISTER_BIT_RW(gpioC)
	REGISTER_BIT_RW(gpioB)
	REGISTER_BIT_RW(gpioA)
};

template<bool Config = false>
struct AHB1Reset : ConfigurationRegister<AHB1Peripheral, Config, AHB1Reset> {
	REGISTER_BIT_C(usb_otg_hs)
	REGISTER_BIT_C(eth_mac)
	REGISTER_BIT_C(dma2)
	REGISTER_BIT_C(dma1)
	REGISTER_BIT_C(crc)
	REGISTER_BIT_C(gpioI)
	REGISTER_BIT_C(gpioH)
	REGISTER_BIT_C(gpioG)
	REGISTER_BIT_C(gpioF)
	REGISTER_BIT_C(gpioE)
	REGISTER_BIT_C(gpioD)
	REGISTER_BIT_C(gpioC)
	REGISTER_BIT_C(gpioB)
	REGISTER_BIT_C(gpioA)
};

template<bool Config = false>
struct AHB1Enable : AHB1Clock<Config, AHB1Enable> {};

template<bool Config = false>
struct AHB1LPEnable : AHB1Clock<Config, AHB1LPEnable> {};

static linker_placed_register<AHB1Reset<>> ahb1_reset [[gnu::weakref(".RCC_AHB1RSTR")]];
static linker_placed_register<AHB1Enable<>> ahb1_enable [[gnu::weakref(".RCC_AHB1ENR")]];
static linker_placed_register<AHB1Enable<>> ahb1_lp_enable [[gnu::weakref(".RCC_AHB1LPENR")]];




enum class AHB2Peripheral : uint32_t {
	usb_oth_fs = 1 << 7,
	rng        = 1 << 6,
	hash       = 1 << 5,
	crypto     = 1 << 4,
	dcmi       = 1 << 0
};

template<bool Config, template<bool> class Inner>
struct AHB2Clock : ConfigurationRegister<AHB2Peripheral, Config, Inner> {
	REGISTER_BIT_RW(usb_oth_fs)
	REGISTER_BIT_RW(rng)
	REGISTER_BIT_RW(hash)
	REGISTER_BIT_RW(crypto)
	REGISTER_BIT_RW(dcmi)
};

template<bool Config = false>
struct AHB2Reset : ConfigurationRegister<AHB2Peripheral, Config, AHB2Reset> {
	REGISTER_BIT_C(usb_oth_fs)
	REGISTER_BIT_C(rng)
	REGISTER_BIT_C(hash)
	REGISTER_BIT_C(crypto)
	REGISTER_BIT_C(dcmi)
};

template<bool Config = false>
struct AHB2Enable : AHB2Clock<Config, AHB2Enable> {};

template<bool Config = false>
struct AHB2LPEnable : AHB2Clock<Config, AHB2LPEnable> {};

static linker_placed_register<AHB2Reset<>> ahb2_reset [[gnu::weakref(".RCC_AHB2RSTR")]];
static linker_placed_register<AHB2Enable<>> ahb2_enable [[gnu::weakref(".RCC_AHB2ENR")]];
static linker_placed_register<AHB2LPEnable<>> ahb2_lp_enable [[gnu::weakref(".RCC_AHB2LPENR")]];




enum class AHB3Peripheral : uint32_t {
	fsmc = 1 << 0
};

template<bool Config, template<bool> class Inner>
struct AHB3Clock : ConfigurationRegister<AHB3Peripheral, Config, Inner> {
	REGISTER_BIT_RW(fsmc)
};

template<bool Config = false>
struct AHB3Reset : ConfigurationRegister<AHB3Peripheral, Config, AHB3Reset> {
	REGISTER_BIT_C(fsmc)
};

template<bool Config = false>
struct AHB3Enable : AHB3Clock<Config, AHB3Enable> {};

template<bool Config = false>
struct AHB3LPEnable : AHB3Clock<Config, AHB3LPEnable> {};

static linker_placed_register<AHB3Reset<>> ahb3_reset [[gnu::weakref(".RCC_AHB3RSTR")]];
static linker_placed_register<AHB3Enable<>> ahb3_enable [[gnu::weakref(".RCC_AHB3ENR")]];
static linker_placed_register<AHB3LPEnable<>> ahb3_lp_enable [[gnu::weakref(".RCC_AHB3LPENR")]];




enum class APB1Peripheral : uint32_t {
	uart8           = 1U << 31,
	uart7           = 1U << 30,
	dac             = 1U << 29,
	power           = 1U << 28,
	can2            = 1U << 26,
	can1            = 1U << 25,
	i2c3            = 1U << 23,
	i2c2            = 1U << 22,
	i2c1            = 1U << 21,
	uart5           = 1U << 20,
	uart4           = 1U << 19,
	usart3          = 1U << 18,
	usart2          = 1U << 17,
	spi3            = 1U << 15,
	spi2            = 1U << 14,
	window_watchdog = 1U << 11,
	timer14         = 1U << 8,
	timer13         = 1U << 7,
	timer12         = 1U << 6,
	timer7          = 1U << 5,
	timer6          = 1U << 4,
	timer5          = 1U << 3,
	timer4          = 1U << 2,
	timer3          = 1U << 1,
	timer2          = 1U << 0
};

template<bool Config, template<bool> class Inner>
struct APB1Clock : ConfigurationRegister<APB1Peripheral, Config, Inner> {
	REGISTER_BIT_RW(uart8)
	REGISTER_BIT_RW(uart7)
	REGISTER_BIT_RW(dac)
	REGISTER_BIT_RW(power)
	REGISTER_BIT_RW(can2)
	REGISTER_BIT_RW(can1)
	REGISTER_BIT_RW(i2c3)
	REGISTER_BIT_RW(i2c2)
	REGISTER_BIT_RW(i2c1)
	REGISTER_BIT_RW(uart5)
	REGISTER_BIT_RW(uart4)
	REGISTER_BIT_RW(usart3)
	REGISTER_BIT_RW(usart2)
	REGISTER_BIT_RW(spi3)
	REGISTER_BIT_RW(spi2)
	REGISTER_BIT_RW(window_watchdog)
	REGISTER_BIT_RW(timer14)
	REGISTER_BIT_RW(timer13)
	REGISTER_BIT_RW(timer12)
	REGISTER_BIT_RW(timer7)
	REGISTER_BIT_RW(timer6)
	REGISTER_BIT_RW(timer5)
	REGISTER_BIT_RW(timer4)
	REGISTER_BIT_RW(timer3)
	REGISTER_BIT_RW(timer2)
};

template<bool Config = false>
struct APB1Reset : ConfigurationRegister<APB1Peripheral, Config, APB1Reset> {
	REGISTER_BIT_C(uart8)
	REGISTER_BIT_C(uart7)
	REGISTER_BIT_C(dac)
	REGISTER_BIT_C(power)
	REGISTER_BIT_C(can2)
	REGISTER_BIT_C(can1)
	REGISTER_BIT_C(i2c3)
	REGISTER_BIT_C(i2c2)
	REGISTER_BIT_C(i2c1)
	REGISTER_BIT_C(uart5)
	REGISTER_BIT_C(uart4)
	REGISTER_BIT_C(usart3)
	REGISTER_BIT_C(usart2)
	REGISTER_BIT_C(spi3)
	REGISTER_BIT_C(spi2)
	REGISTER_BIT_C(window_watchdog)
	REGISTER_BIT_C(timer14)
	REGISTER_BIT_C(timer13)
	REGISTER_BIT_C(timer12)
	REGISTER_BIT_C(timer7)
	REGISTER_BIT_C(timer6)
	REGISTER_BIT_C(timer5)
	REGISTER_BIT_C(timer4)
	REGISTER_BIT_C(timer3)
	REGISTER_BIT_C(timer2)
};

template<bool Config = false>
struct APB1Enable : APB1Clock<Config, APB1Enable> {};

template<bool Config = false>
struct APB1LPEnable : APB1Clock<Config, APB1LPEnable> {};

static linker_placed_register<APB1Reset<>> apb1_reset [[gnu::weakref(".RCC_APB1RSTR")]];
static linker_placed_register<APB1Enable<>> apb1_enable [[gnu::weakref(".RCC_APB1ENR")]];
static linker_placed_register<APB1LPEnable<>> apb1_lp_enable [[gnu::weakref(".RCC_APB1LPENR")]];




enum class APB2Peripheral : uint32_t {
	spi6    = 1U << 21,
	spi5    = 1U << 20,
	timer11 = 1U << 18,
	timer10 = 1U << 17,
	timer9  = 1U << 16,
	syscfg  = 1U << 14,
	spi4    = 1U << 13,
	spi1    = 1U << 12,
	sdio    = 1U << 11,
	adc3    = 1U << 10,
	adc2    = 1U << 9,
	adc1    = 1U << 8,
	usart6  = 1U << 5,
	usart1  = 1U << 4,
	timer8  = 1U << 1,
	timer1  = 1U << 0
};

template<bool Config, template<bool> class Inner>
struct APB2Clock : ConfigurationRegister<APB2Peripheral, Config, Inner> {
	REGISTER_BIT_RW(spi6)
	REGISTER_BIT_RW(spi5)
	REGISTER_BIT_RW(timer11)
	REGISTER_BIT_RW(timer10)
	REGISTER_BIT_RW(timer9)
	REGISTER_BIT_RW(syscfg)
	REGISTER_BIT_RW(spi4)
	REGISTER_BIT_RW(spi1)
	REGISTER_BIT_RW(sdio)
	REGISTER_BIT_RW(adc3)
	REGISTER_BIT_RW(adc2)
	REGISTER_BIT_RW(adc1)
	REGISTER_BIT_RW(usart6)
	REGISTER_BIT_RW(usart1)
	REGISTER_BIT_RW(timer8)
	REGISTER_BIT_RW(timer1)
};

template<bool Config = false>
struct APB2Reset : ConfigurationRegister<APB2Peripheral, Config, APB2Reset> {
	REGISTER_BIT_C(spi6)
	REGISTER_BIT_C(spi5)
	REGISTER_BIT_C(timer11)
	REGISTER_BIT_C(timer10)
	REGISTER_BIT_C(timer9)
	REGISTER_BIT_C(syscfg)
	REGISTER_BIT_C(spi4)
	REGISTER_BIT_C(spi1)
	REGISTER_BIT_C(sdio)
	REGISTER_BIT_C(adc)
	REGISTER_BIT_C(usart6)
	REGISTER_BIT_C(usart1)
	REGISTER_BIT_C(timer8)
	REGISTER_BIT_C(timer1)
};

template<bool Config = false>
struct APB2Enable : APB2Clock<Config, APB2Enable> {};

template<bool Config = false>
struct APB2LPEnable : APB2Clock<Config, APB2LPEnable> {};

static linker_placed_register<APB2Reset<>> apb2_reset [[gnu::weakref(".RCC_APB2RSTR")]];
static linker_placed_register<APB2Enable<>> apb2_enable [[gnu::weakref(".RCC_APB2ENR")]];
static linker_placed_register<APB2LPEnable<>> apb2_lp_enable [[gnu::weakref(".RCC_APB2LPENR")]];




enum class BackupDomainFlags : uint32_t {
	reset       = 1 << 16,
	rtc_enabled = 1 << 15,
	lse_bypass  = 1 << 2,
	lse_ready   = 1 << 1,
	lse_enabled = 1 << 0
};

enum class RTCSource : uint32_t {
	none = 0,
	lse  = 1,
	lsi  = 2,
	hse  = 3
};

template<bool Config = false>
struct BackupDomain : ConfigurationRegister<BackupDomainFlags, Config, BackupDomain> {
	REGISTER_BIT_RW(reset)
	REGISTER_BIT_C(reset)
	REGISTER_BIT_RW(rtc_enabled)
	REGISTER_FIELD_RW(RTCSource, rtc_source, range<9, 8>)
	REGISTER_BIT_RW(lse_bypass)
	REGISTER_BIT_R(lse_ready)
	REGISTER_BIT_RW(lse_enabled)
};

static linker_placed_register<BackupDomain<>> backup_domain [[gnu::weakref(".RCC_BDCR")]];



enum class ClockStatusFlags : uint32_t {
	low_power_reset            = 1U << 31,
	window_watchdog_reset      = 1U << 30,
	independent_watchdog_reset = 1U << 29,
	software_reset             = 1U << 28,
	power_reset                = 1U << 27,
	external_reset             = 1U << 26,
	brownout_reset             = 1U << 25,
	clear_reset_flags          = 1U << 24,

	lsi_ready = 1U << 1,
	lsi_on    = 1U << 0
};

template<bool Config = false>
struct ClockStatus : ConfigurationRegister<ClockStatusFlags, Config, ClockStatus> {
	REGISTER_BIT_R(low_power_reset)
	REGISTER_BIT_R(window_watchdog_reset)
	REGISTER_BIT_R(independent_watchdog_reset)
	REGISTER_BIT_R(software_reset)
	REGISTER_BIT_R(power_reset)
	REGISTER_BIT_R(external_reset)
	REGISTER_BIT_R(brownout_reset)
	REGISTER_BIT_RW(clear_reset_flags)

	REGISTER_BIT_R(lsi_ready)
	REGISTER_BIT_RW(lsi_on)
};

static linker_placed_register<ClockStatus<>> clock_status [[gnu::weakref(".RCC_CSR")]];




enum class SpectrumSpread : uint32_t {
	center = 0,
	down   = 1
};

template<bool Config = false>
struct SpreadSpectrumClock : ConfigurationRegister<void, Config, SpreadSpectrumClock> {
	REGISTER_FIELD_RW(bool, enabled, range<31, 31>)
	REGISTER_FIELD_RW(SpectrumSpread, spread, range<30, 30>)
	REGISTER_INT_RW(step, range<27, 13>)
	REGISTER_INT_RW(period, range<12, 0>)
};

static linker_placed_register<SpreadSpectrumClock<>> spread_spectrum_clock [[gnu::weakref(".RCC_SSCGR")]];




template<bool Config = false>
struct PLLI2S : ConfigurationRegister<void, Config, PLLI2S> {
	REGISTER_INT_RW(r, range<30, 28>)
	REGISTER_INT_RW(n, range<14, 6>)
};

static linker_placed_register<PLLI2S<>> plli2s [[gnu::weakref(".RCC_PLLI2SCFGR")]];




template<bool Config = false>
struct DedicatedClockConfig : ConfigurationRegister<void, Config, DedicatedClockConfig> {
	REGISTER_FIELD_RW(bool, timpre, range<24, 24>)
};

static linker_placed_register<DedicatedClockConfig<>> dedicated_clock_config [[gnu::weakref(".RCC_DCKCFGR")]];




namespace detail {

template<bool Set, typename RCCInfo, template<bool> class Register>
inline void change_in_reg(ConfigurationRegister<typename RCCInfo::type, true, Register>& clock)
{
	if (Set)
		clock.set(RCCInfo::bit);
	else
		clock.clear(RCCInfo::bit);
}

template<bool Set, typename RCCInfo, typename Mismatched, template<bool> class Register>
inline void change_in_reg(ConfigurationRegister<Mismatched, true, Register>&)
{
}

template<typename... T>
inline void ignore(T...) {}

template<bool Set, typename Peripheral, typename... Clocks>
inline void change_one(Clocks&... clocks)
{
	ignore((change_in_reg<Set, typename Peripheral::rcc_info>(clocks), 0)...);
}

template<bool Set, typename... Clocks, typename... Peripherals>
inline void change_many_flights(mpl::list<Peripherals...>, Clocks... clocks)
{
	ignore((change_one<Set, Peripherals>(clocks...), 0)...);
	ignore((clocks.commit(), 0)...);
}

template<bool Set, typename... Clocks, typename... Peripherals>
inline void change_many(mpl::list<Peripherals...> peripherals, Clocks&... clocks)
{
	change_many_flights<Set>(peripherals, clocks.begin()...);
}

}

template<bool Enable = true, typename... Peripherals>
inline void enable(const Peripherals&...)
{
	detail::change_many<Enable>(
		mpl::list<Peripherals...>(),
		ahb1_enable,
		ahb2_enable,
		ahb3_enable,
		apb1_enable,
		apb2_enable);
}

template<bool Enable = true, typename... Peripherals>
inline void lp_enable(const Peripherals&...)
{
	detail::change_many<Enable>(
		mpl::list<Peripherals...>(),
		ahb1_lp_enable,
		ahb2_lp_enable,
		ahb3_lp_enable,
		apb1_lp_enable,
		apb2_lp_enable);
}

template<typename... Peripherals>
inline void reset(const Peripherals&...)
{
	detail::change_many<true>(
		mpl::list<Peripherals...>(),
		ahb1_reset,
		ahb2_reset,
		ahb3_reset,
		apb1_reset,
		apb2_reset);
}

}
}
}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_RCC__HPP_645C98828FBCC4F0 */
