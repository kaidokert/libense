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
		ense::platform::ahb1peripheral_clock.begin()
			.gpioA(true)
			.gpioB(true)
			.gpioC(true)
			.gpioD(true)
			.commit();
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");

		ense::platform::gpio::gpioD.mode().begin()
			.set(0, ense::platform::gpio::PortFunction::output)
			.set(1, ense::platform::gpio::PortFunction::output)
			.set(2, ense::platform::gpio::PortFunction::output)
			.set(3, ense::platform::gpio::PortFunction::output)
			.set(4, ense::platform::gpio::PortFunction::output)
			.set(5, ense::platform::gpio::PortFunction::output)
			.set(6, ense::platform::gpio::PortFunction::output)
			.set(7, ense::platform::gpio::PortFunction::output)
			.set(8, ense::platform::gpio::PortFunction::output)
			.set(9, ense::platform::gpio::PortFunction::output)
			.set(10, ense::platform::gpio::PortFunction::output)
			.set(11, ense::platform::gpio::PortFunction::output)
			.set(12, ense::platform::gpio::PortFunction::output)
			.set(13, ense::platform::gpio::PortFunction::output)
			.set(14, ense::platform::gpio::PortFunction::output)
			.set(15, ense::platform::gpio::PortFunction::output)
			.commit();
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
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

