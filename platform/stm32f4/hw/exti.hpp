#ifndef PLATFORM_STM32F4_HW_EXTI__HPP_76106FFF6AF38822
#define PLATFORM_STM32F4_HW_EXTI__HPP_76106FFF6AF38822

#include <hw/config_struct.hpp>
#include <hw/platform_register_macros.hpp>
#include <hw/config_struct_macros.hpp>

namespace ense {
namespace platform {
namespace exti {

template<bool Config = false>
struct ExtISet : ConfigurationRegister<void, Config, ExtISet> {
	REGISTER_SINGULAR_ARRAY_RW(bool[23], width<1>, range<0, 22>)
};



template<bool Config = false>
struct ExtIPending : ConfigurationRegister<void, Config, ExtIPending> {
	REGISTER_ARRAY_R(bool[23], pending, width<1>, range<0, 22>)
	REGISTER_ARRAY_C(bool[23], set_pending, width<1>, range<0, 22>)
};



namespace detail {

	struct exti_layout {
		ExtISet<> imr;
		ExtISet<> emr;
		ExtISet<> rtsr;
		ExtISet<> ftsr;
		ExtIPending<> swier;
		ExtIPending<> pr;
	};

}

template<typename Flight = void>
struct ExternalInterrupts : ConfigurationStruct<ExternalInterrupts, detail::exti_layout, Flight> {
	template<typename>
	friend struct ExternalInterrupts;

	public:
		typedef detail::exti_layout struct_type;
	private:
		typedef ExternalInterrupts this_type;

	public:
		STRUCT_SINGULAR_ARRAY_RW(interrupt_mask, imr)
		STRUCT_SINGULAR_ARRAY_RW(event_mask, emr)
		STRUCT_SINGULAR_ARRAY_RW(rising_trigger, rtsr)
		STRUCT_SINGULAR_ARRAY_RW(falling_trigger, ftsr)
		STRUCT_ARRAY_C(set_pending, swier, set_pending)
		STRUCT_ARRAY_R(pending, pr, pending)
		STRUCT_ARRAY_C(clear_pending, pr, set_pending)
};

static linker_placed_struct<ExternalInterrupts> exti [[gnu::weakref(".EXTI_Block")]];

}
}
}

#include <hw/config_struct_macros_clear.hpp>
#include <hw/platform_register_macros_clear.hpp>

#endif /* PLATFORM_STM32F4_HW_EXTI__HPP_76106FFF6AF38822 */
