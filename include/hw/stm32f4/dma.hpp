#ifndef INCLUDE_HW_STM32F4_DMA__HPP_2D5FACEC81ECD93C
#define INCLUDE_HW_STM32F4_DMA__HPP_2D5FACEC81ECD93C

#include <hw/config_register.hpp>
#include <hw/config_struct.hpp>
#include <hw/platform_register_macros.hpp>
#include <hw/config_struct_macros.hpp>

namespace ense {
namespace platform {
namespace dma {

enum class DMAInterruptFlags : uint32_t {
	transfer_complete = 1 << 5,
	half_transfer     = 1 << 4,
	transfer_error    = 1 << 3,
	direct_mode_error = 1 << 2,
	fifo_error        = 1 << 0
};

template<bool Config = false>
struct LISR : ConfigurationRegister<DMAInterruptFlags, Config, LISR> {
	REGISTER_BIT_R(transfer_complete)
	REGISTER_BIT_R(half_transfer)
	REGISTER_BIT_R(transfer_error)
	REGISTER_BIT_R(direct_mode_error)
	REGISTER_BIT_R(fifo_error)
};

static_assert(traits::is_platform_register_valid<LISR<>>(), "");



template<bool Config = false>
struct LIFCR : ConfigurationRegister<DMAInterruptFlags, Config, LIFCR> {
	REGISTER_BIT_C(transfer_complete)
	REGISTER_BIT_C(half_transfer)
	REGISTER_BIT_C(transfer_error)
	REGISTER_BIT_C(direct_mode_error)
	REGISTER_BIT_C(fifo_error)
};

static_assert(traits::is_platform_register_valid<LIFCR<>>(), "");



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

enum class DMAConfigFlags : uint32_t {
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
struct SxCR : ConfigurationRegister<DMAConfigFlags, Config, SxCR> {
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

}
}
}

#include <hw/platform_register_macros_clear.hpp>
#include <hw/config_struct_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_DMA__HPP_2D5FACEC81ECD93C */
