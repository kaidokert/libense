#ifndef INCLUDE_HW_STM32F4_RCC__HPP_
#define INCLUDE_HW_STM32F4_RCC__HPP_

#include <stdint.h>

#include <hw/config_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {
namespace platform {

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

template<bool Config = false>
class AHB1PeripheralClockEnable : public ConfigurationRegister<AHB1Peripheral, Config, AHB1PeripheralClockEnable> {
	private:
		typedef AHB1PeripheralClockEnable this_type;

	public:
		REGISTER_BIT_RW(usb_otg_hs_ulpi)
		REGISTER_BIT_RW(usb_otg_hs)
		REGISTER_BIT_RW(eth_mac_ptp)
		REGISTER_BIT_RW(eth_mac_rx)
		REGISTER_BIT_RW(eth_mac_tx)
		REGISTER_BIT_RW(eth_mac)
		REGISTER_BIT_RW(dma2)
		REGISTER_BIT_RW(dma1)
		REGISTER_BIT_RW(ccm_data_ram)
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

extern linker_placed_register<AHB1PeripheralClockEnable<>> ahb1peripheral_clock __attribute__((__weak__, __alias__(".RCC_AHB1ENR")));

}
}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_RCC__HPP_ */
