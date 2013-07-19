#include <string.h>
#include <stdint.h>

#include <hw/stm32f4/rcc.hpp>

#include <hw/cpuid.hpp>
#include <hw/interrupt.hpp>
#include <hw/scs.hpp>
#include <hw/fp.hpp>
#include <hw/systick.hpp>
#include <hw/nvic.hpp>
#include <hw/ivt.hpp>

#define REG(x) *((volatile unsigned int*) (x))

static const unsigned int RCC_BASE = 0x40023800;
static const unsigned int GPIO_A_BASE = 0x40020c00;

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

		// set gpioA9 to output
		REG(GPIO_A_BASE + 0x00) |= 0x55555555;
		// set speed 50MHz
		REG(GPIO_A_BASE + 0x08) |= 0xaaaaaaaa;
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
	ense::shcsr.usage_fault_enabled(true);
//	const char foo[] = "brutzelbums";
	for (unsigned char j = 0; ; j++) {
		uint32_t c = ense::current_exception_number(); {
//		char c = y[j % sizeof(y)]; {
//		char c = j % sizeof(y); {
//		for (char c : foo) {
			for (int i = 0; i < 16; i++) {
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

__attribute__((section("..isr_vectors"), used))
constexpr ense::IVT<
	ense::nmi_handler<nmi>,
	ense::hard_fault_handler<hard>,

	ense::system_handler<ense::SystemHandlerName::MemManage, mem>,
	ense::system_handler<ense::SystemHandlerName::BusFault, bus>,
	ense::system_handler<ense::SystemHandlerName::UsageFault, usage>
> ivt;

// TODO: future-ify all the things

