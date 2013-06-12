#ifndef INCLUDE_HW_STM32F4_RCC__HPP_
#define INCLUDE_HW_STM32F4_RCC__HPP_

#include <stdint.h>

#include <hw/config_register.hpp>

namespace ense {

enum class AHB1Peripheral : uint32_t {
	usb_otg_hs_ulpi = 1 << 30,
	usb_otg_hs = 1 << 29,
	eth_mac_ptp = 1 << 28,
	eth_mac_rx = 1 << 27,
	eth_mac_tx = 1 << 26,
	eth_mac = 1 << 25,
	dma2 = 1 << 22,
	dma1 = 1 << 21,
	ccm_data_ram = 1 << 20,
	backup_sram = 1 << 18,
	crc = 1 << 12,
	gpioI = 1 << 8,
	gpioH = 1 << 7,
	gpioG = 1 << 6,
	gpioF = 1 << 5,
	gpioE = 1 << 4,
	gpioD = 1 << 3,
	gpioC = 1 << 2,
	gpioB = 1 << 1,
	gpioA = 1 << 0
};

template<bool Config = false>
class AHB1PeripheralClockEnable : public ConfigurationRegister<AHB1Peripheral, Config, AHB1PeripheralClockEnable> {
	private:
		typedef AHB1PeripheralClockEnable this_type;

	public:
		REGISTER_BIT(usb_otg_hs_ulpi)
		REGISTER_BIT(usb_otg_hs)
		REGISTER_BIT(eth_mac_ptp)
		REGISTER_BIT(eth_mac_rx)
		REGISTER_BIT(eth_mac_tx)
		REGISTER_BIT(eth_mac)
		REGISTER_BIT(dma2)
		REGISTER_BIT(dma1)
		REGISTER_BIT(ccm_data_ram)
		REGISTER_BIT(backup_sram)
		REGISTER_BIT(crc)
		REGISTER_BIT(gpioI)
		REGISTER_BIT(gpioH)
		REGISTER_BIT(gpioG)
		REGISTER_BIT(gpioF)
		REGISTER_BIT(gpioE)
		REGISTER_BIT(gpioD)
		REGISTER_BIT(gpioC)
		REGISTER_BIT(gpioB)
		REGISTER_BIT(gpioA)
};

extern AHB1PeripheralClockEnable<> ahb1peripheral_clock __attribute__((__weak__, __alias__(".RCC_AHB1ENR")));

}

#endif /* INCLUDE_HW_STM32F4_RCC__HPP_ */
