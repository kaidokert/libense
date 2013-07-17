#ifndef INCLUDE_HW_SYSTICK__HPP_
#define INCLUDE_HW_SYSTICK__HPP_

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {

enum class SystickCSRFlags : uint32_t {
	wrapped = 1 << 16,
	internally_clocked = 1 << 2,
	interrupt_enabled = 1 << 1,
	enable = 1 << 0
};

template<bool Config = false>
class SystickCSR : public ConfigurationRegister<SystickCSRFlags, Config, SystickCSR> {
	private:
		typedef SystickCSR this_type;

	public:
		REGISTER_BIT_R(wrapped)
		REGISTER_BIT_RW(internally_clocked)
		REGISTER_BIT_RW(interrupt_enabled)
		REGISTER_BIT_RW(enable)
};

extern linker_placed_register<SystickCSR<>> sys_csr __attribute__((__weak__, __alias__(".SCS_SystickCSR")));




class SystickRVR : public PlatformRegister<void, SystickRVR, volatile uint32_t> {
	private:
		typedef SystickRVR this_type;

	public:
		REGISTER_INT_R(reload_value, 23, 0)
};

extern linker_placed_register<SystickRVR> sys_rvr __attribute__((__weak__, __alias__(".SCS_SystickRVR")));




class SystickCVR : public WritablePlatformRegister<void, SystickCVR, volatile uint32_t> {
	private:
		typedef SystickCVR this_type;

	public:
		void clear()
		{
			this->value(0);
		}
};

extern linker_placed_register<SystickCVR> sys_cvr __attribute__((__weak__, __alias__(".SCS_SystickCVR")));




enum class SystickCalibFlags : uint32_t {
	reference_present = 1U << 31,
	inexact = 1 << 30
};

class SystickCalib : public PlatformRegister<SystickCalibFlags, SystickCalib, volatile uint32_t> {
	private:
		typedef SystickCalib this_type;

	public:
		REGISTER_BIT_R(reference_present)
		REGISTER_BIT_R(inexact)
		REGISTER_INT_R(ten_ms, 23, 0)
};

extern linker_placed_register<SystickCalib> systickcalib __attribute__((__weak__, __alias__(".SCS_SystickCalib")));

}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_SYSTICK__HPP_ */
