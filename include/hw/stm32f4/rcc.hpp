#ifndef INCLUDE_HW_STM32F4_RCC__HPP_
#define INCLUDE_HW_STM32F4_RCC__HPP_

#include <stdint.h>

#include <hw/config_register.hpp>
#include <hw/platform_register.hpp>

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

static_assert(std::is_standard_layout<AHB1PeripheralClockEnable<>>::value, "");
static_assert(sizeof(AHB1PeripheralClockEnable<>) == sizeof(uint32_t), "");

extern AHB1PeripheralClockEnable<> ahb1peripheral_clock __attribute__((__weak__, __alias__(".RCC_AHB1ENR")));

}

#define __REGISTERS_UNDEF
#include <hw/platform_register.hpp>

#endif /* INCLUDE_HW_STM32F4_RCC__HPP_ */
