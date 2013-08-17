#ifndef INCLUDE_HW_STM32F4_USART__HPP_332391B11B521C28
#define INCLUDE_HW_STM32F4_USART__HPP_332391B11B521C28

#include <stdint.h>

#include <hw/config_register.hpp>
#include <hw/platform_register_macros.hpp>
#include <hw/config_struct.hpp>
#include <hw/config_struct_macros.hpp>

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



template<bool Config = false>
struct GTPR : ConfigurationRegister<void, Config, GTPR> {
	REGISTER_INT_RW(guard_time, detail::bit::range<15, 8>)
	REGISTER_INT_RW(prescaler, detail::bit::range<7, 0>)
};

static_assert(traits::is_platform_register_valid<GTPR<>>(), "");





namespace detail {

	struct layout {
		Status<> status;
		Data<> dr;
		BRR<> brr;
		CR1<> cr1;
		CR2<> cr2;
		CR3<> cr3;
		GTPR<> gtpr;
	};

}

template<typename Flight = void>
struct USART : ConfigurationStruct<USART, detail::layout, Flight> {
	typedef detail::layout struct_type;

	STRUCT_BIT_RW(cts_toggled, status, cts_toggled)
	STRUCT_BIT_RW(lin_break_detected, status, lin_break_detected)
	STRUCT_BIT_R(tdr_empty, status, tdr_empty)
	STRUCT_BIT_RW(tx_complete, status, tx_complete)
	STRUCT_BIT_RW(rdr_not_empty, status, rdr_not_empty)
	STRUCT_BIT_R(idle_detected, status, idle_detected)
	STRUCT_BIT_R(overrun_error, status, overrun_error)
	STRUCT_BIT_R(noise_error, status, noise_error)
	STRUCT_BIT_R(framing_error, status, framing_error)
	STRUCT_BIT_R(parity_error, status, parity_error)

	STRUCT_INT_RW(data, dr, content)

	STRUCT_INT_RW(mantissa, brr, mantissa)
	STRUCT_INT_RW(fraction, brr, fraction)

	STRUCT_BIT_RW(oversample_by_8, cr1, oversample_by_8)
	STRUCT_BIT_RW(enable, cr1, enable)
	STRUCT_BIT_RW(nine_bit_words, cr1, nine_bit_words)
	STRUCT_BIT_RW(wakeup_on_address, cr1, wakeup_on_address)
	STRUCT_BIT_RW(parity_enable, cr1, parity_enable)
	STRUCT_BIT_RW(odd_parity, cr1, odd_parity)
	STRUCT_BIT_RW(parity_interrupt, cr1, parity_interrupt)
	STRUCT_BIT_RW(txe_interrupt, cr1, txe_interrupt)
	STRUCT_BIT_RW(transmit_interrupt, cr1, transmit_interrupt)
	STRUCT_BIT_RW(rxne_interrupt, cr1, rxne_interrupt)
	STRUCT_BIT_RW(idle_interrupt, cr1, idle_interrupt)
	STRUCT_BIT_RW(transmit_enable, cr1, transmit_enable)
	STRUCT_BIT_RW(receive_enable, cr1, receive_enable)
	STRUCT_BIT_RW(receiver_muted, cr1, receiver_muted)
	STRUCT_BIT_C(send_break, cr1, send_break)

	void send_idle()
	{
		transmit_enable(false);
		transmit_enable(true);
	}

	STRUCT_BIT_RW(lin_enabled, cr2, lin_enabled)
	STRUCT_FIELD_RW(stop_bits, cr2, stop_bits)
	STRUCT_BIT_RW(clock_enabled, cr2, clock_enabled)
	STRUCT_BIT_RW(clock_polarity, cr2, clock_polarity)
	STRUCT_BIT_RW(clock_phase, cr2, clock_phase)
	STRUCT_BIT_RW(last_bit_pulse, cr2, last_bit_pulse)
	STRUCT_BIT_RW(lin_break_detect_interrupt, cr2, lin_break_detect_interrupt)
	STRUCT_BIT_RW(lin_break_length, cr2, lin_break_length)
	STRUCT_INT_RW(addr, cr2, addr)

	STRUCT_BIT_RW(sample_one_bit, cr3, sample_one_bit)
	STRUCT_BIT_RW(cts_interrupt, cr3, cts_interrupt)
	STRUCT_BIT_RW(cts_enabled, cr3, cts_enabled)
	STRUCT_BIT_RW(rts_enable, cr3, rts_enable)
	STRUCT_BIT_RW(dma_transmit, cr3, dma_transmit)
	STRUCT_BIT_RW(dma_receive, cr3, dma_receive)
	STRUCT_BIT_RW(smartcard_mode, cr3, smartcard_mode)
	STRUCT_BIT_RW(smartcard_nack, cr3, smartcard_nack)
	STRUCT_BIT_RW(half_duplex, cr3, half_duplex)
	STRUCT_BIT_RW(irda_low_power, cr3, irda_low_power)
	STRUCT_BIT_RW(irda_mode, cr3, irda_mode)
	STRUCT_BIT_RW(error_interrupt, cr3, error_interrupt)

	STRUCT_INT_RW(guard_time, gtpr, guard_time)
	STRUCT_INT_RW(prescaler, gtpr, prescaler)
};

extern linker_placed_struct<USART> usart1 __attribute__((__weak__, __alias__(".USART_USART1")));
extern linker_placed_struct<USART> usart2 __attribute__((__weak__, __alias__(".USART_USART2")));
extern linker_placed_struct<USART> usart3 __attribute__((__weak__, __alias__(".USART_USART3")));
extern linker_placed_struct<USART> uart4 __attribute__((__weak__, __alias__(".UART_USART4")));
extern linker_placed_struct<USART> uart5 __attribute__((__weak__, __alias__(".UART_USART5")));
extern linker_placed_struct<USART> usart6 __attribute__((__weak__, __alias__(".USART_USART6")));
extern linker_placed_struct<USART> usart7 __attribute__((__weak__, __alias__(".USART_USART7")));
extern linker_placed_struct<USART> usart8 __attribute__((__weak__, __alias__(".USART_USART8")));

}
}
}

#include <hw/platform_register_macros_clear.hpp>
#include <hw/config_struct_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_USART__HPP_332391B11B521C28 */
