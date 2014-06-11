#include <string.h>
#include <stdint.h>

#include <hw/rcc.hpp>
#include <hw/gpio.hpp>
#include <hw/syscfg.hpp>
#include <hw/usart.hpp>
#include <hw/dma.hpp>
#include <hw/exti.hpp>

#include <hw/cpuid.hpp>
#include <hw/interrupt.hpp>
#include <hw/scs.hpp>
#include <hw/fp.hpp>
#include <hw/systick.hpp>
#include <hw/nvic.hpp>
#include <hw/ivt.hpp>

namespace {


auto& uart = ense::platform::usart::usart3;
volatile unsigned ticks;


void configure_clocks()
{
	using namespace ense::platform::rcc;

	clock_control.hse_on(true);
	while (!clock_control.hse_ready());
	pll_config.begin()
		.pll_source(PLLSource::hse)
		.n(84 * 2)
		.m(8)
		.p(PLLDivider::div_2)
		.q(8)
		.commit();
	clock_control.pll_on(true);
	while (!clock_control.pll_ready());
	clock_config.begin()
		.clock_source_switch(SystemClockSource::pll)
		.mco1_source(ClockOutSource::pll)
		.apb2_prescaler(APBPrescaler::div_1)
		.commit();
}

void configure_peripherals()
{
	using namespace ense::platform;

	rcc::enable(gpio::gpioA, gpio::gpioC, gpio::gpioD, dma::dma1, usart::usart3);
	gpio::gpioD.begin()
		.configure_range<8, 15>(gpio::Function::output)
		.commit();

	gpio::gpioC.begin()
		.configure_list<10, 11>(gpio::Function::alternate, gpio::OutputType::push_pull, gpio::Speed::fast, gpio::AF(7))
		.commit();

	uart.begin()
		.enable(true)
		.rxne_interrupt(true)
		.mantissa(45).fraction(9)    // 115.2kBaud at 84MHz APB2
//		.mantissa(1666).fraction(12) // 300 baud at 5.25MHz APB2
		.receive_enable(true)
		.transmit_enable(true)
		.commit();

	ense::systick.begin()
		.reload_value(2100000)
		.interrupt_enabled(true)
		.enable(true)
		.commit();
}

void configure_interrupts()
{
	using namespace ense::platform;

	ense::system_handler_priorities[ense::SystemHandlerName::SysTick] = 0x20;

	syscfg::exticr.set(0, syscfg::InterruptSource::portA);
	exti::exti.begin()
		.interrupt_mask(0, true)
		.rising_trigger(0, true)
		.commit();
	ense::nvic.priority(ense::ExternalInterrupt::exti0, 0x40);

	ense::nvic.enable<ense::ExternalInterrupt::exti0, ense::ExternalInterrupt::usart3>();
}



void transmit(char c)
{
	while (!uart.tdr_empty());
	uart.data(c);
}

void transmit(const char* str)
{
	while (*str) {
		transmit(*str);
		str++;
	}
}

void toggle()
{
	ense::platform::gpio::gpioD.out() ^= 1 << 14;
}

void delay(uint32_t loops)
{
	while (loops-- > 0)
		asm ("nop");
}

void blink(uint32_t val, uint8_t bits)
{
	using ense::platform::gpio::gpioD;

	delay(3200000);
	while (bits-- > 0) {
		gpioD.set(((val & 1) << 12) | (1 << 13));
		delay(1600000);
		gpioD.reset((1 << 12) | (1 << 13));
		delay(1600000);
		val >>= 1;
	}
}

void dma_print_banner()
{
	using namespace ense::platform::usart;
	using namespace ense::platform::dma;

	static char buffer[] = "snafu something something\r\n";

	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	namespace d = ense::platform::dma;
	auto assoc = setup_stream(
		d::buffer(buffer, buffer + sizeof(buffer)),
		dma_tx<0>(uart),
		BurstSize::sixteen,
		Priority::very_high,
		FIFOThreshold::full
	);
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");

	assoc.stream().enabled(true);

	uart.dma_transmit(true);
	uart.tx_complete(false);

	while (!assoc.transfer_complete());
	while (!uart.tx_complete());

	uart.dma_transmit(false);
}

void run_usart()
{
	using namespace ense::platform::gpio;
	using namespace ense::platform::usart;

	for (;;) {
		const unsigned val = ticks;

		unsigned cbuf = val;
		char buf[13];
		unsigned c = 12;
		buf[c--] = 0;
		buf[c--] = '\r';
		buf[c--] = '\n';

		while (cbuf) {
			buf[c--] = '0' + (cbuf % 10);
			cbuf /= 10;
		}

		transmit(buf + c + 1);

		while (ticks == val);
	}
}

}

void main()
{
	configure_clocks();
	configure_peripherals();
	configure_interrupts();
	dma_print_banner();

	run_usart();
}

static void lockup() { for (;;) ; }

static void button()
{
	static int i = 0;

	ense::platform::exti::exti.clear_pending(0);
	transmit("exti0 triggered\r\n");
	blink(i++, 4);
}

static void systick()
{
	toggle();
	ticks++;
}

static void echo()
{
	static char buf[64];
	static int pos = 0;

	char rdr = uart.data();

	if (pos == sizeof(buf)) {
		for (unsigned i = 1; i < sizeof(buf); i++)
			buf[i - 1] = buf[i];

		buf[sizeof(buf) - 1] = rdr;
	} else {
		buf[pos++] = rdr;
	}

	transmit('(');
	transmit(buf);
	transmit(")\r\n");
}

[[gnu::section("..isr_vectors"), gnu::used]]
constexpr ense::IVT<
	ense::nmi_handler<lockup>,
	ense::hard_fault_handler<lockup>,

	ense::system_handler<ense::SystemHandlerName::MemManage, lockup>,
	ense::system_handler<ense::SystemHandlerName::BusFault, lockup>,
	ense::system_handler<ense::SystemHandlerName::UsageFault, lockup>,
	ense::system_handler<ense::SystemHandlerName::SysTick, systick>,

	ense::external_interrupt<ense::ExternalInterrupt::usart3, echo>,
	ense::external_interrupt<ense::ExternalInterrupt::exti0, button>
> ivt;
