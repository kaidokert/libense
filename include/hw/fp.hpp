#ifndef INCLUDE_HW_FP__HPP_
#define INCLUDE_HW_FP__HPP_

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {

enum class FPCCRFlags : uint32_t {
	auto_save           = 1U << 31,
	lazy_save           = 1U << 30,
	debug_monitor_ready = 1U << 8,
	bus_fault_ready     = 1U << 6,
	mem_manage_ready    = 1U << 5,
	hard_fault_ready    = 1U << 4,
	thread              = 1U << 3,
	user                = 1U << 1,
	lazy_save_active    = 1U << 0
};

template<bool Config = false>
struct FPCCR : ConfigurationRegister<FPCCRFlags, Config, FPCCR> {
	REGISTER_BIT_RW(auto_save)
	REGISTER_BIT_RW(lazy_save)
	REGISTER_BIT_R(debug_monitor_ready)
	REGISTER_BIT_R(bus_fault_ready)
	REGISTER_BIT_R(mem_manage_ready)
	REGISTER_BIT_R(hard_fault_ready)
	REGISTER_BIT_R(thread)
	REGISTER_BIT_R(user)
	REGISTER_BIT_R(lazy_save_active)
};

extern linker_placed_register<FPCCR<>> fpccr __attribute__((__weak__, __alias__(".SCS_FPCCR")));




extern volatile void* const fpcar __attribute__((__weak__, __alias__(".SCS_FPCAR")));




enum class FPDSCRFlags : uint32_t {
	alternate_half_precision = 1U << 26,
	default_nan              = 1U << 25,
	flush_to_zero            = 1U << 24
};

enum class RoundingMode : uint32_t {
	to_nearest    = 0,
	to_plus_infty = 1,
	to_neg_infty  = 2,
	to_zero       = 3
};

template<bool Config = false>
struct FPDSCR : ConfigurationRegister<FPDSCRFlags, Config, FPDSCR> {
	template<bool>
	friend struct FPSCR;

	REGISTER_BIT_RW(alternate_half_precision)
	REGISTER_BIT_RW(default_nan)
	REGISTER_BIT_RW(flush_to_zero)
	REGISTER_FIELD_RW(RoundingMode, rounding_mode, bit::range<23, 22>)
};

extern linker_placed_register<FPDSCR<>> fpdscr __attribute__((__weak__, __alias__(".SCS_FPDSCR")));

enum class FPSCRFlags : uint32_t {
	negative                 = 1U << 31,
	zero                     = 1U << 30,
	carry                    = 1U << 29,
	overflow                 = 1U << 28,

	alternate_half_precision = 1U << 26,
	default_nan              = 1U << 25,
	flush_to_zero            = 1U << 24,

	input_denormal_exc       = 1U << 7,
	inexact_exc              = 1U << 4,
	underflow_exc            = 1U << 3,
	overflow_exc             = 1U << 2,
	div_by_zero_exc          = 1U << 1,
	invalid_operation_exc    = 1U << 0
};

template<bool Config = false>
struct FPSCR : ConfigurationRegister<FPSCRFlags, Config, FPSCR> {
	uint32_t value() const
	{
		uint32_t result;
		asm volatile ("vmrs %0, FPSCR" : "=r" (result));
		return result;
	}

	FPSCR& value(uint32_t val)
	{
		asm volatile ("vmsr FPSCR, %0" : : "r" (val));
		return *this;
	}

	REGISTER_BIT_R(negative)
	REGISTER_BIT_R(zero)
	REGISTER_BIT_R(carry)
	REGISTER_BIT_R(overflow)

	REGISTER_BIT_RW(alternate_half_precision)
	REGISTER_BIT_RW(default_nan)
	REGISTER_BIT_RW(flush_to_zero)

	REGISTER_FIELD_RW(RoundingMode, rounding_mode, bit::range<23, 22>)

	REGISTER_BIT_R(input_denormal_exc)
	REGISTER_BIT_R(inexact_exc)
	REGISTER_BIT_R(underflow_exc)
	REGISTER_BIT_R(overflow_exc)
	REGISTER_BIT_R(div_by_zero_exc)
	REGISTER_BIT_R(invalid_operation_exc)
};

extern linker_placed_register<FPSCR<>> fpscr __attribute__((__weak__, __alias__(".SCS_FPSCR")));

}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_FP__HPP_ */
