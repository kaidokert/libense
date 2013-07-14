#ifndef INCLUDE_HW_FP__HPP_
#define INCLUDE_HW_FP__HPP_

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {

enum class FPCCRFlags : uint32_t {
	auto_save = 1U << 31,
	lazy_save = 1 << 30,
	debug_monitor_ready = 1 << 8,
	bus_fault_ready = 1 << 6,
	mem_manage_ready = 1 << 5,
	hard_fault_ready = 1 << 4,
	thread = 1 << 3,
	user = 1 << 1,
	lazy_save_active = 1 << 0
};

template<bool Config = false>
class FPCCR : public ConfigurationRegister<FPCCRFlags, Config, FPCCR> {
	private:
		typedef FPCCR this_type;

	public:
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
	alternate_half_precision = 1 << 26,
	default_nan = 1 << 25,
	flush_to_zero = 1 << 24
};

enum class RoundingMode : uint32_t {
	to_nearest = 0,
	to_plus_infty = 1,
	to_neg_infty = 2,
	to_zero = 3
};

template<bool Config = false>
class FPDSCR : public ConfigurationRegister<FPDSCRFlags, Config, FPDSCR> {
	template<bool>
	friend class FPSCR;

	private:
		typedef FPDSCR this_type;

	public:
		REGISTER_BIT_RW(alternate_half_precision)
		REGISTER_BIT_RW(default_nan)
		REGISTER_BIT_RW(flush_to_zero)
		REGISTER_FIELD_RW(RoundingMode, rounding_mode, 23, 22)
};

extern linker_placed_register<FPDSCR<>> fpdscr __attribute__((__weak__, __alias__(".SCS_FPDSCR")));

enum class FPSCRFlags : uint32_t {
	negative = 1U << 31,
	zero = 1 << 30,
	carry = 1 << 29,
	overflow = 1 << 28,

	alternate_half_precision = 1 << 26,
	default_nan = 1 << 25,
	flush_to_zero = 1 << 24,

	input_denormal_exc = 1 << 7,
	inexact_exc = 1 << 4,
	underflow_exc = 1 << 3,
	overflow_exc = 1 << 2,
	div_by_zero_exc = 1 << 1,
	invalid_operation_exc = 1 << 0
};

template<bool Config = false>
class FPSCR : public ConfigurationRegister<FPSCRFlags, Config, FPSCR> {
	private:
		typedef FPSCR this_type;

	public:
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

		REGISTER_FIELD_RW(RoundingMode, rounding_mode, 23, 22)

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
