#ifndef INCLUDE_HW_SCS__HPP_
#define INCLUDE_HW_SCS__HPP_

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {

enum class SCRFlags : uint32_t {
	wakeup_on_pending = 1U << 4,
	sleep_deep        = 1U << 2,
	sleep_on_exit     = 1U << 1
};

template<bool Config = false>
class SCR : public ConfigurationRegister<SCRFlags, Config, SCR> {
	private:
		typedef SCR this_type;

	public:
		REGISTER_BIT_RW(wakeup_on_pending)
		REGISTER_BIT_RW(sleep_deep)
		REGISTER_BIT_RW(sleep_on_exit)
};

extern linker_placed_register<SCR<>> scr __attribute__((__weak__, __alias__(".SCS_SCR")));




enum class CCRFlags : uint32_t {
	stack_align_8byte      = 1U << 9,
	handlers_ignore_fault  = 1U << 8,
	div_0_trap             = 1U << 4,
	unalign_trap           = 1U << 3,
	user_allow_irq_trigger = 1U << 1,
	non_base_thread_enable = 1U << 0
};

template<bool Config = false>
class CCR : public ConfigurationRegister<CCRFlags, Config, CCR> {
	private:
		typedef CCR this_type;

	public:
		REGISTER_BIT_RW(stack_align_8byte)
		REGISTER_BIT_RW(handlers_ignore_fault)
		REGISTER_BIT_RW(div_0_trap)
		REGISTER_BIT_RW(unalign_trap)
		REGISTER_BIT_RW(user_allow_irq_trigger)
		REGISTER_BIT_RW(non_base_thread_enable)
};

extern linker_placed_register<CCR<>> ccr __attribute__((__weak__, __alias__(".SCS_CCR")));




enum class Coprocessor {
	CP0  = 0,
	CP1  = 1,
	CP2  = 2,
	CP3  = 3,
	CP4  = 4,
	CP5  = 5,
	CP6  = 6,
	CP7  = 7,

	CP10 = 10,
	CP11 = 11
};

enum class CoprocessorAccess {
	none       = 0,
	supervisor = 1,
	both       = 3
};

template<bool Config = false>
class CPACR : public ConfigurationRegister<void, Config, CPACR> {
	private:
		typedef CPACR this_type;

	public:
		REGISTER_SINGULAR_ARRAY_RW(CoprocessorAccess[16], bit::index<Coprocessor>, bit::range<0, 31>, bit::width<2>)
		REGISTER_SINGULAR_ARRAY_RW(CoprocessorAccess[16], bit::range<0, 31>, bit::width<2>)
};

extern linker_placed_register<CPACR<>> cpacr __attribute__((__weak__, __alias__(".SCS_CPACR")));

}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_SCS__HPP_ */
