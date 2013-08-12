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

extern "C" {
void __attribute__((__used__)) __aeabi_memset(void *dest, size_t n, int c)
{
	char* p = reinterpret_cast<char*>(dest);
	while (n--) {
		*p++ = c;
	}
}
}

struct X {
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
			.mode_range<8, 15>(ense::platform::gpio::PortFunction::output)
			.commit();
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");

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





		using namespace ense::platform::gpio;
		gpioA.begin()
			.configure(8, PortFunction::alternate, PortOutputType::push_pull, PortSpeed::fast, AF(0))
			.commit();

		gpioB.begin()
			.configure(6, PortFunction::alternate, PortOutputType::push_pull, PortSpeed::fast, AF(7))
			.commit();

		ense::platform::rcc::apb2_enable.usart1(true);
	}

	X()
	{
		foo();
	}
};

X __attribute__((section(".ccmdata"))) x;

char y[] = "\001\002\003\004\005\006\007\010\011\012\013\014\015\016";

static void print()
{
	int i = 0;
	using namespace ense::platform::usart;
	using namespace ense::platform::gpio;
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	usart1.cr1()
		.enable(true)
		.oversample_by_8(true);
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	usart1.baudrate().begin()
		.mantissa(1)
		.fraction(0)
		.commit();
	usart1.cr1()
		.transmit_enable(true);
	usart1.cr2()
		.clock_enabled(false);
	for (;;) {
		usart1.data().content(0x55);
		while (!usart1.status().tx_complete())
			;

		for (int x = 0; x < 3000000; x++)
			asm volatile ("nop");
	}
	ense::shcsr.usage_fault_enabled(true);
//	const char foo[] = "brutzelbums";
	for (unsigned char j = 0; ; j++) {
		uint32_t c = ense::current_exception_number(); {
//		char c = y[j % sizeof(y)]; {
//		char c = j % sizeof(y); {
//		for (char c : foo) {
			for (int i = 0; i < 16; i++) {
				gpioD.odr.value(((c & 1) << 12) | (1 << 13));
				for (int i = 0; i < 400000; i++)
					__asm__ __volatile__ ("nop");
				gpioD.odr.value(0);
				for (int i = 0; i < 400000; i++)
					__asm__ __volatile__ ("nop");
				c >>= 1;
			}
			gpioD.odr.value(1 << 14);
			for (int i = 0; i < 400000; i++)
				__asm__ __volatile__ ("nop");
			gpioD.odr.value(0);
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

__attribute__((section("..isr_vectors"), used))
constexpr ense::IVT<
	ense::nmi_handler<nmi>,
	ense::hard_fault_handler<hard>,

	ense::system_handler<ense::SystemHandlerName::MemManage, mem>,
	ense::system_handler<ense::SystemHandlerName::BusFault, bus>,
	ense::system_handler<ense::SystemHandlerName::UsageFault, usage>
> ivt;

// TODO: future-ify all the things

