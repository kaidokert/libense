#include <string.h>
#include <stdint.h>

#include <hw/stm32f4/rcc.hpp>
#include <hw/cpuid.hpp>
#include <hw/interrupt.hpp>
#include <hw/scs.hpp>
#include <hw/fp.hpp>

#define REG(x) *((volatile unsigned int*) (x))

static const unsigned int RCC_BASE = 0x40023800;
static const unsigned int GPIO_A_BASE = 0x40020c00;

struct X {
	static void foo()
	{
		// enable gpioA clock
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		ense::system_handler_priorities.set(ense::SystemHandlerName::SysTick, 0xff);
		ense::ahb1peripheral_clock.begin()
			.gpioA(true)
			.gpioB(true)
			.gpioC(true)
			.gpioD(true)
			.commit();
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		ense::fpscr.rounding_mode(ense::RoundingMode::to_neg_infty);

		// set gpioA9 to output
		REG(GPIO_A_BASE + 0x00) |= 0x55555555;
		// set speed 50MHz
		REG(GPIO_A_BASE + 0x08) |= 0xaaaaaaaa;
	}

	X()
	{
		foo();
		volatile float d = 0.4;
		if (4.30f * d > 0.1f) {
			asm volatile ("nop");
			ense::cpacr[ense::Coprocessor::CP10] = ense::CoprocessorAccess::both;
			asm volatile ("nop");
		}
	}
};

X __attribute__((section(".ccmdata"))) x;

char y[] = "\001\002\003\004\005\006\007\010\011\012\013\014\015\016";

static void print()
{
	ense::shcsr.usage_fault_enabled(true);
//	const char foo[] = "brutzelbums";
	for (unsigned char j = 0; ; j++) {
		uint32_t c = static_cast<uint32_t>(ense::cpacr[ense::Coprocessor::CP10]); {
//		char c = y[j % sizeof(y)]; {
//		char c = j % sizeof(y); {
//		for (char c : foo) {
			for (int i = 0; i < 32; i++) {
				REG(GPIO_A_BASE + 0x14) = ((c & 1) << 12) | (1 << 13);
				for (int i = 0; i < 400000; i++)
					__asm__ __volatile__ ("nop");
				REG(GPIO_A_BASE + 0x14) = 0;
				for (int i = 0; i < 400000; i++)
					__asm__ __volatile__ ("nop");
				c >>= 1;
			}
			REG(GPIO_A_BASE + 0x14) = 1 << 14;
			for (int i = 0; i < 400000; i++)
				__asm__ __volatile__ ("nop");
			REG(GPIO_A_BASE + 0x14) = 0;
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

extern "C" {
	extern void _start();

	__attribute__((section("..isr_vectors"), used))
	void (*const isr_vectors[])() {
		_start,
		nmi,
		hard,
		mem,
		bus,
		usage
	};
}


// TODO: future-ify all the things

