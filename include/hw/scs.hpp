#ifndef INCLUDE_HW_SCS__HPP_
#define INCLUDE_HW_SCS__HPP_

#include <hw/platform_register.hpp>

namespace ense {

enum class SCRFlags : uint32_t {
	wakeup_on_pending = 1 << 4,
	sleep_deep = 1 << 2,
	sleep_on_exit = 1 << 1
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
	stack_align_8byte = 1 << 9,
	handlers_ignore_fault = 1 << 8,
	div_0_trap = 1 << 4,
	unalign_trap = 1 << 3,
	user_allow_irq_trigger = 1 << 1,
	non_base_thread_enable = 1 << 0
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

}

#define __REGISTERS_UNDEF
#include <hw/platform_register.hpp>

#endif /* INCLUDE_HW_SCS__HPP_ */
