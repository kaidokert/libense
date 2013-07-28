#include <string.h>
#include <stdint.h>

#include <hw/stm32f4/rcc.hpp>
#include <hw/stm32f4/gpio.hpp>

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
		ense::platform::gpio::gpioD.mode()
			.set_range<0, 15>(ense::platform::gpio::PortFunction::output);

		ense::platform::gpio::gpioA.mode()
			.set(8, ense::platform::gpio::PortFunction::alternate);
		ense::platform::gpio::gpioA.output_type()
			.set(8, ense::platform::gpio::PortOutputType::push_pull);
		ense::platform::gpio::gpioA.speed()
			.set(8, ense::platform::gpio::PortSpeed::fast);
		ense::platform::gpio::gpioA.afh()
			.set(0, 0);

		ense::platform::gpio::gpioC.mode()
			.set(9, ense::platform::gpio::PortFunction::alternate);
		ense::platform::gpio::gpioC.output_type()
			.set(9, ense::platform::gpio::PortOutputType::push_pull);
		ense::platform::gpio::gpioC.speed()
			.set(9, ense::platform::gpio::PortSpeed::fast);
		ense::platform::gpio::gpioC.afh()
			.set(1, 0);
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
			.n(80)
			.m(32)
			.p(PLLDivider::div_8)
			.q(8)
			.commit();
		clock_control.begin()
			.pll_on(true)
			.commit();
		while (!clock_control.pll_ready())
			;
		spread_spectrum_clock.begin()
			.enabled(true)
			.spread(SpectrumSpread::center)
			.step(1000)
			.period(999)
			.commit();
		clock_config.begin()
			.clock_source_switch(SystemClockSource::pll)
			.mco1_source(ClockOutSource::pll)
			.commit();
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
	using namespace ense::platform::gpio;
	ense::shcsr.usage_fault_enabled(true);
//	const char foo[] = "brutzelbums";
	for (unsigned char j = 0; ; j++) {
		uint32_t c = ense::current_exception_number(); {
//		char c = y[j % sizeof(y)]; {
//		char c = j % sizeof(y); {
//		for (char c : foo) {
			for (int i = 0; i < 16; i++) {
				gpioD.output().value(((c & 1) << 12) | (1 << 13));
				for (int i = 0; i < 400000; i++)
					__asm__ __volatile__ ("nop");
				gpioD.output().value(0);
				for (int i = 0; i < 400000; i++)
					__asm__ __volatile__ ("nop");
				c >>= 1;
			}
			gpioD.output().value(1 << 14);
			for (int i = 0; i < 400000; i++)
				__asm__ __volatile__ ("nop");
			gpioD.output().value(0);
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

