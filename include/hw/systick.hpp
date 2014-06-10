#ifndef INCLUDE_HW_SYSTICK__HPP_23D4D4AD4A9496DD
#define INCLUDE_HW_SYSTICK__HPP_23D4D4AD4A9496DD

#include <hw/platform_register.hpp>
#include <hw/config_struct.hpp>
#include <hw/platform_register_macros.hpp>
#include <hw/config_struct_macros.hpp>

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




template<bool Config = false>
struct SystickRVR : ConfigurationRegister<void, Config, SystickRVR> {
	REGISTER_INT_RW(reload_value, range<23, 0>)
};




template<bool Config = false>
struct SystickCVR : ConfigurationRegister<void, Config, SystickCVR> {
	REGISTER_INT_RW(value, range<23, 0>)

	SystickCVR& clear()
	{
		return this->value(0);
	}
};




enum class SystickCalibFlags : uint32_t {
	reference_present = 1U << 31,
	inexact           = 1U << 30
};

template<bool Config = false>
struct SystickCalib : ConfigurationRegister<void, Config, SystickCalib> {
	REGISTER_BIT_R(reference_present)
	REGISTER_BIT_R(inexact)
	REGISTER_INT_R(ten_ms, range<23, 0>)
};



namespace detail {

	struct systick_layout {
		SystickCSR<> csr;
		SystickRVR<> rvr;
		SystickCVR<> cvr;
		SystickCalib<> calib;
	};

}

template<typename Flight = void>
struct Systick : ConfigurationStruct<Systick, detail::systick_layout, Flight> {
	template<typename>
	friend struct Systick;

	public:
		typedef detail::systick_layout struct_type;
	private:
		typedef Systick this_type;

	public:
		STRUCT_BIT_R(wrapped, csr, wrapped)
		STRUCT_BIT_RW(internally_clocked, csr, internally_clocked)
		STRUCT_BIT_RW(interrupt_enabled, csr, interrupt_enabled)
		STRUCT_BIT_RW(enable, csr, enable)

		STRUCT_FIELD_RW(reload_value, rvr, reload_value)

		STRUCT_FIELD_RW(value, cvr, value)

		STRUCT_FIELD_R(reference_present, calib, reference_present)
		STRUCT_FIELD_R(inexact, calib, inexact)
		STRUCT_FIELD_R(ten_ms, calib, ten_ms)
};

static linker_placed_struct<Systick> systick [[gnu::weakref(".SCS_Systick")]];

}

#include <hw/config_struct_macros_clear.hpp>
#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_SYSTICK__HPP_23D4D4AD4A9496DD */
