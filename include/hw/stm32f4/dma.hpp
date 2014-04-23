#ifndef INCLUDE_HW_STM32F4_DMA__HPP_2D5FACEC81ECD93C
#define INCLUDE_HW_STM32F4_DMA__HPP_2D5FACEC81ECD93C

#include <hw/config_register.hpp>
#include <hw/config_struct.hpp>
#include <hw/platform_register_macros.hpp>
#include <hw/config_struct_macros.hpp>

namespace ense {
namespace platform {
namespace dma {

enum class InterruptFlags : uint32_t {
	transfer_complete = 1 << 5,
	half_transfer     = 1 << 4,
	transfer_error    = 1 << 3,
	direct_mode_error = 1 << 2,
	fifo_error        = 1 << 0
};

template<bool Config = false>
struct ISR : ConfigurationRegister<uint32_t[2], Config, ISR> {
	REGISTER_SINGULAR_ARRAY_R(InterruptFlags[8], detail::bit::width<6>, detail::bit::range<0, 63>, detail::bit::element_offsets<0, 6, 16, 22, 32, 38, 48, 54>)
};

static_assert(traits::is_platform_register_valid<ISR<>>(), "");



template<bool Config = false>
struct IFCR : ConfigurationRegister<uint32_t[2], Config, IFCR> {
	REGISTER_SINGULAR_ARRAY_W(InterruptFlags[8], detail::bit::width<6>, detail::bit::range<0, 63>, detail::bit::element_offsets<0, 6, 16, 22, 32, 38, 48, 54>)
};

static_assert(traits::is_platform_register_valid<IFCR<>>(), "");



enum class BurstSize : uint32_t {
	one     = 0,
	four    = 1,
	eight   = 2,
	sixteen = 3
};

enum class Priority : uint32_t {
	low       = 0,
	medium    = 1,
	high      = 2,
	very_high = 3
};

enum class DataSize : uint32_t {
	byte     = 0,
	halfword = 1,
	word     = 2
};

enum class Direction : uint32_t {
	peripheral_to_memory = 0,
	memory_to_peripheral = 1,
	memory_to_memory     = 2
};

enum class ConfigFlags : uint32_t {
	double_buffered              = 1 << 18,
	peripheral_increment_by_four = 1 << 15,
	memory_increment             = 1 << 10,
	peripheral_increment         = 1 << 9,
	circular                     = 1 << 8,
	peripheral_flow_control      = 1 << 5,

	transfer_complete_interrupt  = 1 << 4,
	half_transfer_interrupt      = 1 << 3,
	transfer_error_interrupt     = 1 << 2,
	directmode_error_interrupt   = 1 << 1,
	fifo_error_interrupt         = 1 << 0
};

template<bool Config = false>
struct SCR : ConfigurationRegister<ConfigFlags, Config, SCR> {
	REGISTER_INT_RW(channel, detail::bit::range<27, 25>)
	REGISTER_FIELD_RW(BurstSize, memory_burst_size, detail::bit::range<24, 23>)
	REGISTER_FIELD_RW(BurstSize, peripheral_burst_size, detail::bit::range<22, 21>)
	REGISTER_INT_RW(current_target, detail::bit::range<19, 19>)
	REGISTER_BIT_RW(double_buffered)
	REGISTER_FIELD_RW(Priority, priority, detail::bit::range<17, 16>)
	REGISTER_BIT_RW(peripheral_increment_by_four)
	REGISTER_FIELD_RW(DataSize, memory_size, detail::bit::range<14, 13>)
	REGISTER_FIELD_RW(DataSize, peripheral_size, detail::bit::range<12, 11>)
	REGISTER_BIT_RW(memory_increment)
	REGISTER_BIT_RW(peripheral_increment)
	REGISTER_BIT_RW(circular)
	REGISTER_FIELD_RW(Direction, direction, detail::bit::range<7, 6>)
	REGISTER_BIT_RW(peripheral_flow_control)
	REGISTER_BIT_RW(transfer_complete_interrupt)
	REGISTER_BIT_RW(half_transfer_interrupt)
	REGISTER_BIT_RW(transfer_error_interrupt)
	REGISTER_BIT_RW(direct_error_interrupt)
	REGISTER_BIT_RW(fifo_error_interrupt)
};

static_assert(traits::is_platform_register_valid<SCR<>>(), "");



template<bool Config = false>
struct SNDTR : ConfigurationRegister<void, Config, SNDTR> {
	REGISTER_INT_RW(count, detail::bit::range<15, 0>)
};

static_assert(traits::is_platform_register_valid<SNDTR<>>(), "");



template<bool Config = false>
struct SAR : ConfigurationRegister<void, Config, SAR> {
	void* address() const
	{
		static_assert(sizeof(void*) == sizeof(this->value()), "");
		return reinterpret_cast<void*>(this->value());
	}

	SAR& address(void* val)
	{
		static_assert(sizeof(void*) == sizeof(this->value()), "");
		return this->value(reinterpret_cast<uint32_t>(val));
	}
};

static_assert(traits::is_platform_register_valid<SAR<>>(), "");



enum class SFCRFlags : uint32_t {
	error_interrupt_enable = 1U << 7,
	direct_mode_disable    = 1U << 2
};

enum class FIFOStatus : uint32_t {
	lt_25pct       = 0,
	lt_50pct       = 1,
	lt_75pct       = 2,
	lt_100pct      = 3,
	at_100pct      = 4,
	at_0pct        = 5,
};

enum class FIFOThreshold : uint32_t {
	quarter        = 0,
	half           = 1,
	three_quarters = 2,
	full           = 3
};

template<bool Config = false>
struct SFCR : ConfigurationRegister<SFCRFlags, Config, SFCR> {
	REGISTER_BIT_RW(error_interrupt_enable)
	REGISTER_FIELD_R(FIFOStatus, fifo_status, detail::bit::range<5, 3>)
	REGISTER_BIT_RW(direct_mode_disable)
	REGISTER_FIELD_RW(FIFOThreshold, fifo_threshold, detail::bit::range<1, 0>)
};

static_assert(traits::is_platform_register_valid<SFCR<>>(), "");



namespace detail {

	struct layout_interrupts {
		ISR<> isr;
		IFCR<> ifcr;
	};

}

template<typename Flight = void>
struct Interrupts : ConfigurationStruct<Interrupts, detail::layout_interrupts, Flight> {
	template<typename>
	friend struct Interrupts;

	public:
		typedef detail::layout_interrupts struct_type;
	private:
		typedef Interrupts this_type;
		typedef Flight flight_type;
		template<typename Next>
		using this_template = Interrupts<Next>;

	public:
		STRUCT_SINGULAR_ARRAY_R(status, isr)
		STRUCT_SINGULAR_ARRAY_W(clear, ifcr)
};

static_assert(traits::is_config_struct_valid<Interrupts>(), "");



namespace detail {

	struct layout_stream {
		SCR<> scr;
		SNDTR<> sndtr;
		SAR<> spar;
		SAR<> m0ar;
		SAR<> m1ar;
		SFCR<> sfcr;
	};

}

template<typename Flight = void>
struct Stream : ConfigurationStruct<Stream, detail::layout_stream, Flight> {
	template<typename>
	friend struct Stream;

	public:
		typedef detail::layout_stream struct_type;
	private:
		typedef Stream this_type;
		typedef Flight flight_type;
		template<typename Next>
		using this_template = Stream<Next>;

	public:
		STRUCT_FIELD_RW(channel, scr, channel)
		STRUCT_FIELD_RW(memory_burst_size, scr, memory_burst_size)
		STRUCT_FIELD_RW(peripheral_burst_size, scr, peripheral_burst_size)
		STRUCT_FIELD_RW(current_target, scr, current_target)
		STRUCT_BIT_RW(double_buffered, scr, double_buffered)
		STRUCT_FIELD_RW(priority, scr, priority)
		STRUCT_BIT_RW(peripheral_increment_by_four, scr, peripheral_increment_by_four)
		STRUCT_FIELD_RW(memory_size, scr, memory_size)
		STRUCT_FIELD_RW(peripheral_size, scr, peripheral_size)
		STRUCT_BIT_RW(memory_increment, scr, memory_increment)
		STRUCT_BIT_RW(peripheral_increment, scr, peripheral_increment)
		STRUCT_BIT_RW(circular, scr, circular)
		STRUCT_FIELD_RW(direction, scr, direction)
		STRUCT_BIT_RW(peripheral_flow_control, scr, peripheral_flow_control)
		STRUCT_BIT_RW(transfer_complete_interrupt, scr, transfer_complete_interrupt)
		STRUCT_BIT_RW(half_transfer_interrupt, scr, half_transfer_interrupt)
		STRUCT_BIT_RW(transfer_error_interrupt, scr, transfer_error_interrupt)
		STRUCT_BIT_RW(direct_error_interrupt, scr, direct_error_interrupt)
		STRUCT_BIT_RW(fifo_error_interrupt, scr, fifo_error_interrupt)

		STRUCT_FIELD_RW(count, sndtr, count)

		STRUCT_FIELD_RW(peripheral_address, spar, address)

		STRUCT_FIELD_RW(memory_address_0, m0ar, address)

		STRUCT_FIELD_RW(memory_address_1, m1ar, address)

		STRUCT_BIT_RW(error_interrupt_enable, sfcr, error_interrupt_enable)
		STRUCT_FIELD_R(fifo_status, sfcr, fifo_status)
		STRUCT_BIT_RW(direct_mode_disable, sfcr, direct_mode_disable)
		STRUCT_FIELD_RW(fifo_threshold, sfcr, fifo_threshold)
};

static_assert(traits::is_config_struct_valid<Stream>(), "");



struct DMA {
	Interrupts<> interrupts;
	Stream<> stream[8];
};

static_assert(std::is_standard_layout<DMA>::value, "");
static_assert(sizeof(DMA) == sizeof(Interrupts<>::struct_type) + sizeof(Stream<>::struct_type[8]), "");

static DMA dma1 [[gnu::weakref(".DMA_DMA1")]];
static DMA dma2 [[gnu::weakref(".DMA_DMA2")]];

}
}
}

#include <hw/platform_register_macros_clear.hpp>
#include <hw/config_struct_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_DMA__HPP_2D5FACEC81ECD93C */
