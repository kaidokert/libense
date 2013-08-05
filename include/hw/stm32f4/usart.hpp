#ifndef INCLUDE_HW_STM32F4_USART__HPP_
#define INCLUDE_HW_STM32F4_USART__HPP_

#include <stdint.h>

#include <hw/config_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {
namespace platform {
namespace usart {

enum class StatusFlags : uint32_t {
	cts_toggled        = 1 << 9,
	lin_break_detected = 1 << 8,
	tdr_empty          = 1 << 7,
	tx_complete        = 1 << 6,
	rdr_no_empty       = 1 << 5,
	idle_detected      = 1 << 4,
	overrun_error      = 1 << 3,
	noise_error        = 1 << 2,
	framing_error      = 1 << 1,
	parity_error       = 1 << 0
};

template<bool Config = false>
struct Status : ConfigurationRegister<StatusFlags, Config, Status> {
	REGISTER_BIT_RW(cts_toggled)
	REGISTER_BIT_RW(lin_break_detected)
	REGISTER_BIT_R(tdr_empty)
	REGISTER_BIT_RW(tx_complete)
	REGISTER_BIT_RW(rdr_not_empty)
	REGISTER_BIT_R(idle_detected)
	REGISTER_BIT_R(overrun_error)
	REGISTER_BIT_R(noise_error)
	REGISTER_BIT_R(framing_error)
	REGISTER_BIT_R(parity_error)
};

static_assert(traits::is_platform_register_valid<Status<>>(), "");



template<bool Config = false>
struct Data : ConfigurationRegister<void, Config, Data> {
	REGISTER_INT_RW(content, detail::bit::range<8, 0>)
};

static_assert(traits::is_platform_register_valid<Data<>>(), "");




template<bool Config = false>
struct BRR : ConfigurationRegister<void, Config, BRR> {
	using ConfigurationRegister<void, Config, usart::BRR>::value;

	REGISTER_INT_RW(mantissa, detail::bit::range<15, 4>)
	REGISTER_INT_RW(fraction, detail::bit::range<3, 0>)

	BRR& value(uint32_t mantissa, uint32_t fraction)
	{
		this->value(((mantissa << 4) & 0xFFF0) | (fraction & 0xF));
		return *this;
	}
};

static_assert(traits::is_platform_register_valid<BRR<>>(), "");




enum class CR1Flags : uint32_t {
	oversample_by_8    = 1 << 15,
	enable             = 1 << 13,
	nine_bit_words     = 1 << 12,
	wakeup_on_address  = 1 << 11,
	parity_enable      = 1 << 10,
	odd_parity         = 1 << 9,
	parity_interrupt   = 1 << 8,
	txe_interrupt      = 1 << 7,
	transmit_interrupt = 1 << 6,
	rxne_interrupt     = 1 << 5,
	idle_interrupt     = 1 << 4,
	transmit_enable    = 1 << 3,
	receive_enable     = 1 << 2,
	receiver_muted     = 1 << 1,
	send_break         = 1 << 0
};

template<bool Config = false>
struct CR1 : ConfigurationRegister<CR1Flags, Config, CR1> {
	REGISTER_BIT_RW(oversample_by_8)
	REGISTER_BIT_RW(enable)
	REGISTER_BIT_RW(nine_bit_words)
	REGISTER_BIT_RW(wakeup_on_address)
	REGISTER_BIT_RW(parity_enable)
	REGISTER_BIT_RW(odd_parity)
	REGISTER_BIT_RW(parity_interrupt)
	REGISTER_BIT_RW(txe_interrupt)
	REGISTER_BIT_RW(transmit_interrupt)
	REGISTER_BIT_RW(rxne_interrupt)
	REGISTER_BIT_RW(idle_interrupt)
	REGISTER_BIT_RW(transmit_enable)
	REGISTER_BIT_RW(receive_enable)
	REGISTER_BIT_RW(receiver_muted)
	REGISTER_BIT_C(send_break)

	CR1& send_idle()
	{
		transmit_enable(false);
		transmit_enable(true);
		return *this;
	}
};

static_assert(traits::is_platform_register_valid<CR1<>>(), "");




enum class CR2Flags : uint32_t {
	lin_enabled = 1 << 14,
	clock_enabled = 1 << 11,
	clock_polarity = 1 << 10,
	clock_phase = 1 << 9,
	last_bit_pulse = 1 << 8,
	lin_break_detect_interrupt = 1 << 6,
	lin_break_length = 1 << 5
};

enum class StopBits : uint32_t {
	one      = 0,
	half     = 1,
	two      = 2,
	one_half = 3
};

template<bool Config = false>
struct CR2 : ConfigurationRegister<CR2Flags, Config, CR2> {
	REGISTER_BIT_RW(lin_enabled)
	REGISTER_FIELD_RW(StopBits, stop_bits, detail::bit::range<13, 12>)
	REGISTER_BIT_RW(clock_enabled)
	REGISTER_BIT_RW(clock_polarity)
	REGISTER_BIT_RW(clock_phase)
	REGISTER_BIT_RW(last_bit_pulse)
	REGISTER_BIT_RW(lin_break_detect_interrupt)
	REGISTER_BIT_RW(lin_break_length)
	REGISTER_INT_RW(addr, detail::bit::range<3, 0>)
};

static_assert(traits::is_platform_register_valid<CR2<>>(), "");



enum class CR3Flags : uint32_t {
	sample_one_bit = 1 << 11,
	cts_interrupt = 1 << 10,
	cts_enabled = 1 << 9,
	rts_enable = 1 << 8,
	dma_transmit = 1 < 7,
	dma_receive = 1 << 6,
	smartcard_mode = 1 << 5,
	smartcard_nack = 1 << 4,
	half_duplex = 1 << 3,
	irda_low_power = 1 << 2,
	irda_mode = 1 << 1,
	error_interrupt = 1 << 0
};

template<bool Config = false>
struct CR3 : ConfigurationRegister<CR3Flags, Config, CR3> {
	REGISTER_BIT_RW(sample_one_bit)
	REGISTER_BIT_RW(cts_interrupt)
	REGISTER_BIT_RW(cts_enabled)
	REGISTER_BIT_RW(rts_enable)
	REGISTER_BIT_RW(dma_transmit)
	REGISTER_BIT_RW(dma_receive)
	REGISTER_BIT_RW(smartcard_mode)
	REGISTER_BIT_RW(smartcard_nack)
	REGISTER_BIT_RW(half_duplex)
	REGISTER_BIT_RW(irda_low_power)
	REGISTER_BIT_RW(irda_mode)
	REGISTER_BIT_RW(error_interrupt)
};

static_assert(traits::is_platform_register_valid<CR3<>>(), "");





class USART {
	private:
		Status<> _status;
		Data<> _data;
		BRR<> _brr;
		CR1<> _cr1;
		CR2<> _cr2;
		CR3<> _cr3;

	public:
		Status<>& status() { return _status; }

		Data<>& data() { return _data; }

		BRR<>& baudrate() { return _brr; }

		CR1<>& cr1() { return _cr1; }

		CR2<>& cr2() { return _cr2; }

		CR3<>& cr3() { return _cr3; }
};

static_assert(std::is_standard_layout<USART>(), "");

extern USART usart1 __attribute__((__weak__, __alias__(".USART_USART1")));
extern USART usart2 __attribute__((__weak__, __alias__(".USART_USART2")));
extern USART usart3 __attribute__((__weak__, __alias__(".USART_USART3")));
extern USART uart4 __attribute__((__weak__, __alias__(".UART_USART4")));
extern USART uart5 __attribute__((__weak__, __alias__(".UART_USART5")));
extern USART usart6 __attribute__((__weak__, __alias__(".USART_USART6")));
extern USART usart7 __attribute__((__weak__, __alias__(".USART_USART7")));
extern USART usart8 __attribute__((__weak__, __alias__(".USART_USART8")));

}
}
}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_UART__HPP_ */
