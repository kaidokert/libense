#ifndef INCLUDE_HW_STM32F4_RCC__HPP_
#define INCLUDE_HW_STM32F4_RCC__HPP_

#include <stdint.h>

namespace ense {

class AHB1PeripheralClockEnable {
	private:
		volatile uint32_t _value;

	public:
		uint32_t value() const { return _value; }

		enum class Bits : uint32_t {
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

		bool get(Bits bit) const {
			return (_value & static_cast<uint32_t>(bit)) != 0;
		}

		AHB1PeripheralClockEnable& set(Bits bit, bool value) {
			if (value) {
				_value |= static_cast<uint32_t>(bit);
			} else {
				_value &= ~static_cast<uint32_t>(bit);
			}
			return *this;
		}

#define ACCESS(name) \
		bool name() const { return get(Bits::name); } \
		AHB1PeripheralClockEnable& name(bool enable) { return set(Bits::name, enable); }

		ACCESS(usb_otg_hs_ulpi)
		ACCESS(usb_otg_hs)
		ACCESS(eth_mac_ptp)
		ACCESS(eth_mac_rx)
		ACCESS(eth_mac_tx)
		ACCESS(eth_mac)
		ACCESS(dma2)
		ACCESS(dma1)
		ACCESS(ccm_data_ram)
		ACCESS(backup_sram)
		ACCESS(crc)
		ACCESS(gpioI)
		ACCESS(gpioH)
		ACCESS(gpioG)
		ACCESS(gpioF)
		ACCESS(gpioE)
		ACCESS(gpioD)
		ACCESS(gpioC)
		ACCESS(gpioB)
		ACCESS(gpioA)
};

extern AHB1PeripheralClockEnable ahb1peripheral_clock __attribute__((__weak__, __alias__(".RCC_AHB1ENR")));

}

#endif /* INCLUDE_HW_STM32F4_RCC__HPP_ */
