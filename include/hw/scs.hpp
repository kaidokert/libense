#ifndef INCLUDE_HW_SCS__HPP_
#define INCLUDE_HW_SCS__HPP_

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

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




enum class Coprocessor {
	CP0 = 0,
	CP1 = 2,
	CP2 = 4,
	CP3 = 6,
	CP4 = 8,
	CP5 = 10,
	CP6 = 12,
	CP7 = 14,

	CP10 = 20,
	CP11 = 22
};

enum class CoprocessorAccess {
	none = 0,
	supervisor = 1,
	both = 3
};

template<bool Config = false>
class CPACR : public ConfigurationRegister<void, Config, CPACR> {
	private:
		typedef CPACR this_type;

		static constexpr uint32_t cp_value(Coprocessor cp, CoprocessorAccess acc) { return static_cast<uint32_t>(acc) << static_cast<uint32_t>(cp); }
		static constexpr uint32_t cp_mask(Coprocessor cp) { return cp_value(cp, CoprocessorAccess::both); }

		struct array_wrapper {
			CPACR* cpacr;
			Coprocessor cp;

			array_wrapper(CPACR* cpacr, Coprocessor cp)
				: cpacr(cpacr), cp(cp)
			{
			}

			operator CoprocessorAccess() const { return cpacr->get(cp); }

			template<typename T>
			operator T() const { return static_cast<T>(cpacr->get(cp)); }

			CoprocessorAccess operator=(CoprocessorAccess acc)
			{
				cpacr->set(cp, acc);
				return acc;
			}
		};

	public:
		CPACR& set(Coprocessor cp, CoprocessorAccess acc)
		{
			this->value((this->value() & ~cp_mask(cp)) | cp_value(cp, acc));
			return *this;
		}

		CoprocessorAccess get(Coprocessor cp) const { return static_cast<CoprocessorAccess>((this->value() & cp_mask(cp)) >> static_cast<uint32_t>(cp)); }

		CoprocessorAccess operator[](Coprocessor cp) const { return get(cp); }

		array_wrapper operator[](Coprocessor cp) { return array_wrapper(this, cp); }
};

extern linker_placed_register<CPACR<>> cpacr __attribute__((__weak__, __alias__(".SCS_CPACR")));

}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_SCS__HPP_ */
