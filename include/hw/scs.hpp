#ifndef INCLUDE_HW_SCS__HPP_F219D4CB2F9EBE0D
#define INCLUDE_HW_SCS__HPP_F219D4CB2F9EBE0D

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {

enum class SCRFlags : uint32_t {
	wakeup_on_pending = 1U << 4,
	sleep_deep        = 1U << 2,
	sleep_on_exit     = 1U << 1
};

template<bool Config = false>
struct SCR : ConfigurationRegister<SCRFlags, Config, SCR> {
	REGISTER_BIT_RW(wakeup_on_pending)
	REGISTER_BIT_RW(sleep_deep)
	REGISTER_BIT_RW(sleep_on_exit)
};

static linker_placed_register<SCR<>> scr [[gnu::weakref(".SCS_SCR")]];




enum class CCRFlags : uint32_t {
	stack_align_8byte      = 1U << 9,
	handlers_ignore_fault  = 1U << 8,
	div_0_trap             = 1U << 4,
	unalign_trap           = 1U << 3,
	user_allow_irq_trigger = 1U << 1,
	non_base_thread_enable = 1U << 0
};

template<bool Config = false>
struct CCR : ConfigurationRegister<CCRFlags, Config, CCR> {
	REGISTER_BIT_RW(stack_align_8byte)
	REGISTER_BIT_RW(handlers_ignore_fault)
	REGISTER_BIT_RW(div_0_trap)
	REGISTER_BIT_RW(unalign_trap)
	REGISTER_BIT_RW(user_allow_irq_trigger)
	REGISTER_BIT_RW(non_base_thread_enable)
};

static linker_placed_register<CCR<>> ccr [[gnu::weakref(".SCS_CCR")]];




enum class CoprocessorAccess {
	none       = 0,
	supervisor = 1,
	both       = 3
};

template<bool Config = false>
struct CPACR : ConfigurationRegister<void, Config, CPACR> {
	REGISTER_SINGULAR_ARRAY_RW(CoprocessorAccess[16], range<0, 31>, width<2>)
};

static linker_placed_register<CPACR<>> cpacr [[gnu::weakref(".SCS_CPACR")]];

}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_SCS__HPP_F219D4CB2F9EBE0D */
