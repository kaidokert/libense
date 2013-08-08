#ifndef INCLUDE_HW_SYSTICK__HPP_23D4D4AD4A9496DD
#define INCLUDE_HW_SYSTICK__HPP_23D4D4AD4A9496DD

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {

enum class SystickCSRFlags : uint32_t {
	wrapped            = 1U << 16,
	internally_clocked = 1U << 2,
	interrupt_enabled  = 1U << 1,
	enable             = 1U << 0
};

template<bool Config = false>
struct SystickCSR : ConfigurationRegister<SystickCSRFlags, Config, SystickCSR> {
	REGISTER_BIT_R(wrapped)
	REGISTER_BIT_RW(internally_clocked)
	REGISTER_BIT_RW(interrupt_enabled)
	REGISTER_BIT_RW(enable)
};

extern linker_placed_register<SystickCSR<>> sys_csr __attribute__((__weak__, __alias__(".SCS_SystickCSR")));




struct SystickRVR : WritablePlatformRegister<void, SystickRVR, volatile uint32_t> {
	REGISTER_INT_RW(reload_value, detail::bit::range<23, 0>)
};

extern linker_placed_register<SystickRVR> sys_rvr __attribute__((__weak__, __alias__(".SCS_SystickRVR")));




struct SystickCVR : WritablePlatformRegister<void, SystickCVR, volatile uint32_t> {
	void clear()
	{
		this->value(0);
	}
};

extern linker_placed_register<SystickCVR> sys_cvr __attribute__((__weak__, __alias__(".SCS_SystickCVR")));




enum class SystickCalibFlags : uint32_t {
	reference_present = 1U << 31,
	inexact           = 1U << 30
};

struct SystickCalib : PlatformRegister<SystickCalibFlags, SystickCalib, volatile uint32_t> {
	REGISTER_BIT_R(reference_present)
	REGISTER_BIT_R(inexact)
	REGISTER_INT_R(ten_ms, detail::bit::range<23, 0>)
};

extern linker_placed_register<SystickCalib> systickcalib __attribute__((__weak__, __alias__(".SCS_SystickCalib")));

}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_SYSTICK__HPP_23D4D4AD4A9496DD */
