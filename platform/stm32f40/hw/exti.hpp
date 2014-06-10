#ifndef PLATFORM_STM32F40_HW_EXTI__HPP_BD19CAA6F5DE91C4
#define PLATFORM_STM32F40_HW_EXTI__HPP_BD19CAA6F5DE91C4

#include "../../stm32f4/hw/exti.hpp"

namespace ense {

enum class ExternalInterrupt : uint32_t {
	window_watchdog              = 0,
	pvd                          = 1,

	tamper                       = 2,
	timestamp                    = 2,
	tamper__timestamp            = 2,

	rtc_wakeup                   = 3,
	flash                        = 4,
	rcc                          = 5,
	exti0                        = 6,
	exti1                        = 7,
	exti2                        = 8,
	exti3                        = 9,
	exti4                        = 10,
	dma1_stream0                 = 11,
	dma1_stream1                 = 12,
	dma1_stream2                 = 13,
	dma1_stream3                 = 14,
	dma1_stream4                 = 15,
	dma1_stream5                 = 16,
	dma1_stream6                 = 17,
	adc                          = 18,
	can1_tx                      = 19,
	can1_rx0                     = 20,
	can1_rx1                     = 21,
	can1_sce                     = 22,
	exti5to9                     = 23,

	timer1_break                 = 24,
	timer9                       = 24,
	timer1_break__timer9         = 24,

	timer1_up                    = 25,
	timer10                      = 25,
	timer1_up__timer10           = 25,

	timer1_trigger               = 26,
	timer1_comm                  = 26,
	timer11                      = 26,
	timer1_trigger_comm__timer11 = 26,

	timer1_capture_compare       = 27,
	timer2                       = 28,
	timer3                       = 29,
	timer4                       = 30,
	i2c1_event                   = 31,
	i2c1_error                   = 32,
	i2c2_event                   = 33,
	i2c2_error                   = 34,
	spi1                         = 35,
	spi2                         = 36,
	usart1                       = 37,
	usart2                       = 38,
	usart3                       = 39,
	exti10to15                   = 40,
	rtc_alarm                    = 41,
	otg_fs_wakeup                = 42,

	timer8_break                 = 43,
	timer12                      = 43,
	timer8_break__timer12        = 43,

	timer8_up                    = 44,
	timer13                      = 44,
	timer8_up__timer13           = 44,

	timer8_trigger               = 45,
	timer8_comm                  = 45,
	timer14                      = 45,
	timer8_trigger_comm__timer14 = 45,

	timer8_capture_compare       = 46,
	dma1_stream7                 = 47,
	fsmc                         = 48,
	sdio                         = 49,
	timer5                       = 50,
	spi3                         = 51,
	uart4                        = 52,
	uart5                        = 53,

	timer6                       = 54,
	dac_underrun                 = 54,
	timer6__dac_underrun         = 54,

	timer7                       = 55,
	dma2_stream0                 = 56,
	dma2_stream1                 = 57,
	dma2_stream2                 = 58,
	dma2_stream3                 = 59,
	dma2_stream4                 = 60,
	ethernet                     = 61,
	ethernet_wakeup              = 62,
	can2_tx                      = 63,
	can2_rx0                     = 64,
	can2_rx1                     = 65,
	can2_sce                     = 66,
	otg_fs                       = 67,
	dma2_stream5                 = 68,
	dma2_stream6                 = 69,
	dma2_stream7                 = 70,
	usart6                       = 71,
	i2c3_event                   = 72,
	i2c3_error                   = 73,
	otg_hs_ep1_out               = 74,
	otg_hs_ep1_in                = 75,
	otg_hs_wakeup                = 76,
	otg_hs                       = 77,
	dcmi                         = 78,
	crypto                       = 79,

	hash                         = 80,
	rng                          = 80,
	hash__rng                    = 80,

	fpu                          = 81
};

}

#endif /* PLATFORM_STM32F40_HW_EXTI__HPP_BD19CAA6F5DE91C4 */
