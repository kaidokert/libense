#ifndef INCLUDE_HW_STM32F4_DMA__HPP_2D5FACEC81ECD93C
#define INCLUDE_HW_STM32F4_DMA__HPP_2D5FACEC81ECD93C

#include <hw/rcc.hpp>
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
	fifo_error        = 1 << 0,

	all               = transfer_complete | half_transfer | transfer_error |
	                    direct_mode_error | fifo_error
};

template<bool Config = false>
struct ISR : ConfigurationRegister<uint32_t[2], Config, ISR> {
	REGISTER_SINGULAR_ARRAY_R(InterruptFlags[8], width<6>, range<0, 63>, element_offsets<0, 6, 16, 22, 32, 38, 48, 54>)

	REGISTER_ARRAY_R(bool[8], transfer_complete, width<6>, range<0, 63>, element_offsets<5, 11, 21, 27, 37, 43, 53, 59>)
	REGISTER_ARRAY_R(bool[8], half_transfer, width<6>, range<0, 63>, element_offsets<4, 10, 20, 26, 36, 42, 52, 58>)
	REGISTER_ARRAY_R(bool[8], transfer_error, width<6>, range<0, 63>, element_offsets<3, 9, 19, 25, 35, 41, 51, 57>)
	REGISTER_ARRAY_R(bool[8], direct_mode_error, width<6>, range<0, 63>, element_offsets<2, 8, 18, 24, 34, 40, 50, 56>)
	REGISTER_ARRAY_R(bool[8], fifo_error, width<6>, range<0, 63>, element_offsets<0, 6, 16, 22, 32, 38, 48, 54>)
};

static_assert(traits::is_platform_register_valid<ISR<>>(), "");



template<bool Config = false>
struct IFCR : ConfigurationRegister<uint32_t[2], Config, IFCR> {
	static constexpr bool is_write_only = true;

	REGISTER_SINGULAR_ARRAY_W(InterruptFlags[8], width<6>, range<0, 63>, element_offsets<0, 6, 16, 22, 32, 38, 48, 54>)
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
	enabled                      = 1 << 0
};

template<bool Config = false>
struct SCR : ConfigurationRegister<ConfigFlags, Config, SCR> {
	REGISTER_INT_RW(channel, range<27, 25>)
	REGISTER_FIELD_RW(BurstSize, memory_burst_size, range<24, 23>)
	REGISTER_FIELD_RW(BurstSize, peripheral_burst_size, range<22, 21>)
	REGISTER_INT_RW(current_target, range<19, 19>)
	REGISTER_BIT_RW(double_buffered)
	REGISTER_FIELD_RW(Priority, priority, range<17, 16>)
	REGISTER_BIT_RW(peripheral_increment_by_four)
	REGISTER_FIELD_RW(DataSize, memory_size, range<14, 13>)
	REGISTER_FIELD_RW(DataSize, peripheral_size, range<12, 11>)
	REGISTER_BIT_RW(memory_increment)
	REGISTER_BIT_RW(peripheral_increment)
	REGISTER_BIT_RW(circular)
	REGISTER_FIELD_RW(Direction, direction, range<7, 6>)
	REGISTER_BIT_RW(peripheral_flow_control)
	REGISTER_BIT_RW(transfer_complete_interrupt)
	REGISTER_BIT_RW(half_transfer_interrupt)
	REGISTER_BIT_RW(transfer_error_interrupt)
	REGISTER_BIT_RW(directmode_error_interrupt)
	REGISTER_BIT_RW(enabled)

	REGISTER_INT_RW(_interrupt_mask, range<1, 4>)
};

static_assert(traits::is_platform_register_valid<SCR<>>(), "");



template<bool Config = false>
struct SNDTR : ConfigurationRegister<void, Config, SNDTR> {
	static constexpr bool can_elide_read_on_modify = true;

	REGISTER_INT_RW(count, range<15, 0>)
};

static_assert(traits::is_platform_register_valid<SNDTR<>>(), "");



template<bool Config = false>
struct SAR : ConfigurationRegister<void, Config, SAR> {
	static constexpr bool can_elide_read_on_modify = true;

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
	REGISTER_FIELD_R(FIFOStatus, fifo_status, range<5, 3>)
	REGISTER_BIT_RW(direct_mode_disable)
	REGISTER_FIELD_RW(FIFOThreshold, fifo_threshold, range<1, 0>)
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

	public:
		STRUCT_SINGULAR_ARRAY_R(status, isr)
		STRUCT_SINGULAR_ARRAY_W(clear, ifcr)

		STRUCT_ARRAY_R(transfer_complete, isr, transfer_complete)
		STRUCT_ARRAY_R(half_transfer, isr, half_transfer)
		STRUCT_ARRAY_R(transfer_error, isr, transfer_error)
		STRUCT_ARRAY_R(direct_mode_error, isr, direct_mode_error)
		STRUCT_ARRAY_R(fifo_error, isr, fifo_error)
};



namespace detail {

	struct layout_stream {
		SCR<> scr;
		SNDTR<> sndtr;
		SAR<> par;
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
		STRUCT_BIT_RW(directmode_error_interrupt, scr, directmode_error_interrupt)
		STRUCT_BIT_RW(enabled, scr, enabled)

		STRUCT_FIELD_RW(count, sndtr, count)

		STRUCT_FIELD_RW(peripheral_address, par, address)

		STRUCT_FIELD_RW(memory_address_0, m0ar, address)

		STRUCT_FIELD_RW(memory_address_1, m1ar, address)

		STRUCT_BIT_RW(error_interrupt_enable, sfcr, error_interrupt_enable)
		STRUCT_FIELD_R(fifo_status, sfcr, fifo_status)
		STRUCT_BIT_RW(direct_mode_disable, sfcr, direct_mode_disable)
		STRUCT_FIELD_RW(fifo_threshold, sfcr, fifo_threshold)

		STRUCT_FIELD_RW(_interrupt_mask, scr, _interrupt_mask)
};



struct DMA {
	linker_placed_struct<Interrupts> interrupts;
	linker_placed_struct<Stream> stream[8];
};

static_assert(std::is_standard_layout<DMA>::value, "");
static_assert(sizeof(DMA) == sizeof(Interrupts<>::struct_type) + sizeof(Stream<>::struct_type[8]), "");

template<typename PeripheralType, PeripheralType Bit>
struct DMAInst : DMA {
	struct rcc_info {
		typedef PeripheralType type;
		static constexpr PeripheralType bit = Bit;
	};
};

static DMAInst<ense::platform::rcc::AHB1Peripheral, ense::platform::rcc::AHB1Peripheral::dma1> dma1 [[gnu::weakref(".DMA_DMA1")]];
static DMAInst<ense::platform::rcc::AHB1Peripheral, ense::platform::rcc::AHB1Peripheral::dma2> dma2 [[gnu::weakref(".DMA_DMA2")]];



namespace detail {

	template<typename Info>
	inline DMA* select_controller()
	{
		switch (Info::controller) {
			case 1: return &dma1;
			case 2: return &dma2;
			default: return nullptr;
		}
	}

}


template<typename Info>
struct association {
	static constexpr auto stream_num = Info::stream;

	DMA& controller() const
	{
		return *detail::select_controller<Info>();
	}

	Stream<>& stream() const
	{
		return controller().stream[stream_num];
	}

	bool transfer_complete() const { return controller().interrupts.transfer_complete(stream_num); }
	bool half_transfer()     const { return controller().interrupts.half_transfer(stream_num); }
	bool transfer_error()    const { return controller().interrupts.transfer_error(stream_num); }
	bool direct_mode_error() const { return controller().interrupts.direct_mode_error(stream_num); }
	bool fifo_error()        const { return controller().interrupts.fifo_error(stream_num); }
};

template<DataSize Size, BurstSize BurstSize, bool Inc, bool Inc4, bool FlowControl>
struct target {
	static constexpr auto size = Size;
	static constexpr auto burst_size = BurstSize;
	static constexpr bool inc_by_four = Inc;
	static constexpr bool increment = Inc4;
	static constexpr bool flow_control = FlowControl;

	void* const address;

	constexpr target(void* address)
		: address(address)
	{}
};

template<unsigned Controller, unsigned Stream, unsigned Channel, DataSize Size, BurstSize BurstSize, bool Inc, bool Inc4, bool FlowControl>
struct info : target<Size, BurstSize, Inc, Inc4, FlowControl> {
	static constexpr auto channel = Channel;
	static constexpr auto stream = Stream;
	static constexpr auto controller = Controller;

	constexpr info(void* address)
		: target<Size, BurstSize, Inc, Inc4, FlowControl>(address)
	{}
};

template<unsigned Controller, unsigned Stream, unsigned Channel>
struct trigger {
	static constexpr auto channel = Channel;
	static constexpr auto stream = Stream;
	static constexpr auto controller = Controller;

	template<DataSize Size, BurstSize BurstSize, bool Inc, bool Inc4, bool FlowControl>
	friend info<Controller, Stream, Channel, Size, BurstSize, Inc, Inc4, FlowControl>
		operator>>(const trigger&, const target<Size, BurstSize, Inc, Inc4, FlowControl>& target)
	{
		return { target.address };
	}
};



namespace detail {

	template<typename Item>
	struct data_size {
		static_assert(sizeof(Item) == 1 || sizeof(Item) == 2 || sizeof(Item) == 4, "Buffer item size not in {1, 2, 4}");

		static constexpr DataSize size = sizeof(Item) == 1 ? DataSize::byte : (sizeof(Item) == 2 ? DataSize::halfword : DataSize::word);
	};

	template<bool IsConst, bool Double, bool Circular, bool Pinned, DataSize Size>
	struct dma_buffer {
		static constexpr bool double_buffer = Double;
		static constexpr bool circular_buffer = Circular;
		static constexpr bool pinned_buffer = Pinned;
		static constexpr DataSize size = Size;

		typedef typename std::conditional<IsConst, const void*, void*>::type ptr_type;

		ptr_type const address;
		uint32_t const length;
		ptr_type const alternate_address;

		constexpr dma_buffer(ptr_type address, uint32_t length, ptr_type alternate_address = nullptr)
			: address(address), length(length), alternate_address(alternate_address)
		{}
	};


	template<bool IsConst, bool Double, bool Circular, bool Pinned, DataSize Size>
	constexpr auto circular(dma_buffer<IsConst, Double, Circular, Pinned, Size> buffer)
	{
		return dma_buffer<IsConst, Double, true, Pinned, Size>(buffer.address, buffer.length, buffer.alternate_address);
	}

	template<bool IsConst, bool Double, bool Circular, bool Pinned, DataSize Size>
	constexpr auto pinned(dma_buffer<IsConst, Double, Circular, Pinned, Size> buffer)
	{
		return dma_buffer<IsConst, Double, Circular, true, Size>(buffer.address, buffer.length, buffer.alternate_address);
	}

}

template<typename Item, DataSize ItemSize = detail::data_size<Item>::size>
constexpr auto buffer(Item* begin, Item* end)
	-> typename std::enable_if<
		!std::is_pointer<Item>::value,
		detail::dma_buffer<std::is_const<Item>::value, false, false, false, ItemSize>
	>::type
{
	static_assert(std::is_standard_layout<Item>::value, "DMA buffer not standard-layout");

	return detail::dma_buffer<std::is_const<Item>::value, false, false, false, ItemSize>(begin, end - begin);
}

template<typename Item, DataSize ItemSize = detail::data_size<Item>::size>
constexpr auto buffer(Item* begin, Item* end, Item* alternate)
	-> typename std::enable_if<
		!std::is_pointer<Item>::value,
		detail::dma_buffer<std::is_const<Item>::value, true, false, false, ItemSize>
	>::type
{
	static_assert(std::is_standard_layout<Item>::value, "DMA buffer not standard-layout");

	return detail::dma_buffer<std::is_const<Item>::value, true, false, false, ItemSize>(begin, end - begin, alternate);
}



namespace detail {

	template<typename Flight>
	inline Stream<Flight> apply_addr_1(Stream<Flight> stream, void*, std::false_type)
	{ return stream; }

	template<typename Flight>
	inline auto apply_addr_1(Stream<Flight> stream, void* addr, std::true_type)
	{ return stream.memory_address_1(addr); }

	template<typename Buffer, typename Flight>
	inline auto apply_buffer(Stream<Flight> stream, Buffer buffer, DataSize psize)
	{
		return
			apply_addr_1(
				stream
					.current_target(0)
					.double_buffered(Buffer::double_buffer)
					.memory_size(Buffer::size)
					.memory_increment(!Buffer::pinned_buffer)
					.circular(Buffer::circular_buffer)
					.count((buffer.length << static_cast<uint32_t>(Buffer::size)) >> static_cast<uint32_t>(psize))
					.memory_address_0((void*) buffer.address),
				(void*) buffer.alternate_address,
				std::integral_constant<bool, Buffer::double_buffer>());
	}

	template<typename Flight>
	inline auto apply(Stream<Flight> stream, BurstSize burst)
	{ return stream.direct_mode_disable(true).memory_burst_size(burst); }

	template<typename Flight>
	inline auto apply(Stream<Flight> stream, Priority prio)
	{ return stream.priority(prio); }

	template<typename Flight>
	inline auto apply(Stream<Flight> stream, InterruptFlags irq)
	{
		const uint32_t mask = static_cast<uint32_t>(irq);
		return stream
			._interrupt_mask(mask >> 1)
			.error_interrupt_enable(mask & static_cast<uint32_t>(InterruptFlags::fifo_error));
	}

	template<typename Flight>
	inline auto apply(Stream<Flight> stream, FIFOThreshold thresh)
	{ return stream.fifo_threshold(thresh); }

	template<typename Flight>
	inline auto apply(Stream<Flight> stream, Direction dir)
	{ return stream.direction(dir); }


	template<typename Flight, typename Arg, typename... Args>
	inline auto apply(Stream<Flight> stream, Arg arg, Args... args)
	{
		return apply(apply(stream, arg), args...);
	}


	template<typename Info, typename Flight>
	inline auto apply_dma_info(Stream<Flight> stream, const Info& info)
	{
		return
			stream
				.peripheral_burst_size(Info::burst_size)
				.peripheral_increment_by_four(Info::inc_by_four)
				.peripheral_increment(Info::increment)
				.peripheral_flow_control(Info::flow_control)
				.peripheral_address(info.address)
				.channel(Info::channel)
				.peripheral_size(Info::size);
	}

	template<typename Flight>
	inline auto prepare(Stream<Flight> stream)
	{
		return stream
			.transfer_complete_interrupt(false)
			.half_transfer_interrupt(false)
			.transfer_error_interrupt(false)
			.directmode_error_interrupt(false)
			.error_interrupt_enable(false);
	}



	template<typename Buffer, typename Info, typename... Args>
	inline association<Info> setup_stream(const Buffer& buf, Info&& info, Direction dir, Args... args)
	{
		DMA& dma = *detail::select_controller<Info>();
		Stream<>& stream = dma.stream[Info::stream];

		stream.enabled(false);
		while (stream.enabled());

		dma.interrupts.clear(Info::stream, InterruptFlags::all);
		detail::apply(
				detail::apply_buffer(
					detail::apply_dma_info(
						detail::prepare(stream.begin_reset()),
						info),
					buf,
					Info::size),
				args...)
			.direction(dir)
			.commit();

		return {};
	}

}

template<bool IsConst, bool Double, bool Circular, bool Pinned, DataSize Size, typename Info, typename... Args>
inline auto setup_stream(const detail::dma_buffer<IsConst, Double, Circular, Pinned, Size>& buf, Info&& info, Args... args)
{
	return detail::setup_stream(buf, std::move(info), Direction::memory_to_peripheral, args...);
}

template<bool IsConst, bool Double, bool Circular, bool Pinned, DataSize Size, typename Info, typename... Args>
inline auto setup_stream(Info&& info, const detail::dma_buffer<IsConst, Double, Circular, Pinned, Size>& buf, Args... args)
{
	static_assert(!IsConst, "cannot DMA to const buffer");
	return detail::setup_stream(buf, std::move(info), Direction::peripheral_to_memory, args...);
}

}
}
}

#include <hw/platform_register_macros_clear.hpp>
#include <hw/config_struct_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_DMA__HPP_2D5FACEC81ECD93C */
