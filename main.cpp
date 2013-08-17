#include <string.h>
#include <stdint.h>

#include <hw/stm32f4/rcc.hpp>
#include <hw/stm32f4/gpio.hpp>
#include <hw/stm32f4/usart.hpp>

#include <hw/cpuid.hpp>
#include <hw/interrupt.hpp>
#include <hw/scs.hpp>
#include <hw/fp.hpp>
#include <hw/systick.hpp>
#include <hw/nvic.hpp>
#include <hw/ivt.hpp>

struct X {
	[[gnu::noinline]]
	static void configure_pll()
	{
		using namespace ense::platform::rcc;
		clock_control.begin()
			.hse_on(true)
			.commit();
		while (!clock_control.hse_ready())
			;
		pll_config.begin()
			.pll_source(PLLSource::hse)
			.n(84 * 2)
			.m(8)
			.p(PLLDivider::div_2)
			.q(8)
			.commit();
		clock_control.begin()
			.pll_on(true)
			.commit();
		while (!clock_control.pll_ready())
			;
		clock_config.begin()
			.clock_source_switch(SystemClockSource::pll)
			.mco1_source(ClockOutSource::pll)
			.apb2_prescaler(APBPrescaler::div_1)
			.commit();
	}

	static void foo()
	{
		// enable gpioA clock
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		ense::system_handler_priorities.set(ense::SystemHandlerName::SysTick, 0xff);
		ense::platform::rcc::ahb1_enable.begin()
			.gpioA(true)
			.gpioB(true)
			.gpioC(true)
			.gpioD(true)
			.commit();
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		ense::nvic.enable<
			ense::ExternalInterrupt::a,
			ense::ExternalInterrupt::x>();
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		ense::platform::gpio::gpioD.begin()
			.mode_range<8, 15>(ense::platform::gpio::Function::output)
			.commit();
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");

		configure_pll();

		using namespace ense::platform::gpio;
		gpioA.begin()
			.configure(8, Function::alternate, OutputType::push_pull, Speed::fast, AF(0))
			.commit();

		gpioB.begin()
			.configure(6, Function::alternate, OutputType::push_pull, Speed::fast, AF(7))
			.commit();

		ense::platform::rcc::apb2_enable.usart1(true);
	}

	X()
	{
		foo();
	}
};

X x [[gnu::section(".ccmdata")]];

char y[] = "\001\002\003\004\005\006\007\010\011\012\013\014\015\016";

static void print()
{
	using namespace ense::platform::usart;
	using namespace ense::platform::gpio;
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	usart1.begin()
		.enable(true)
		.oversample_by_8(true)
		.mantissa(1)
		.fraction(0)
		.transmit_enable(true)
		.clock_enabled(false)
		.commit();
	for (uint8_t i = 0;; i++) {
		usart1.data(i++);
		while (!usart1.tdr_empty())
			;
//		gpioD.out() ^= 1 << 12;

//		for (int x = 0; x < 500000; x++)
//			asm volatile ("nop");
	}
	ense::shcsr.usage_fault_enabled(true);
//	const char foo[] = "brutzelbums";
	for (unsigned char j = 0; ; j++) {
		uint32_t c = ense::current_exception_number(); {
//		char c = y[j % sizeof(y)]; {
//		char c = j % sizeof(y); {
//		for (char c : foo) {
			for (int i = 0; i < 16; i++) {
				gpioD.out() = ((c & 1) << 12) | (1 << 13);
				for (int i = 0; i < 400000; i++)
					__asm__ __volatile__ ("nop");
				gpioD.out() = 0;
				for (int i = 0; i < 400000; i++)
					__asm__ __volatile__ ("nop");
				c >>= 1;
			}
			gpioD.out() = 1 << 14;
			for (int i = 0; i < 400000; i++)
				__asm__ __volatile__ ("nop");
			gpioD.out() = 0;
			for (int i = 0; i < 1200000; i++)
				__asm__ __volatile__ ("nop");
		}
	}
}

void main()
{
	print();
}

void nmi() { for (;;) ; }
void hard() { for (;;) ; }
void usage() { for (;;) ; }
void bus() { for (;;) ; }
void mem() { for (;;) ; }

[[gnu::section("..isr_vectors"), gnu::used]]
constexpr ense::IVT<
	ense::nmi_handler<nmi>,
	ense::hard_fault_handler<hard>,

	ense::system_handler<ense::SystemHandlerName::MemManage, mem>,
	ense::system_handler<ense::SystemHandlerName::BusFault, bus>,
	ense::system_handler<ense::SystemHandlerName::UsageFault, usage>
> ivt;

// TODO: future-ify all the things

