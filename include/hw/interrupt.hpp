#ifndef INCLUDE_HW_INTERRUPT__HPP_
#define INCLUDE_HW_INTERRUPT__HPP_

#include <hw/platform_register.hpp>

namespace ense {

class InterruptControllerType {
	private:
		volatile uint32_t value;

	public:
		uint32_t lines() const
		{
			uint32_t line_code = value & 0xF;
			return line_code == 0xf ? 496 : 32 * (line_code + 1);
		}
};

static_assert(std::is_standard_layout<InterruptControllerType>::value, "");
static_assert(sizeof(InterruptControllerType) == sizeof(uint32_t), "");

extern InterruptControllerType ict __attribute__((__weak__, __alias__(".SCS_ICTR")));




extern volatile uint32_t actlr __attribute__((__weak__, __alias__(".SCS_ACTLR")));




inline void trigger_interrupt(uint8_t num)
{
	extern volatile uint32_t stir __attribute__((__weak__, __alias__(".SCS_STIR")));
	stir = num;
}




enum class ICSRFlags : uint32_t {
	nmi_pending = 1U << 31,
	pendsv_pending = 1U << 28,
	pendsv_clear = 1U << 27,
	systick_pending = 1U << 26,
	systick_clear = 1U << 25,
	isr_preempt = 1U << 23,
	isr_pending = 1U << 22,
	ret_to_base = 1U << 11
};

class ICSR : public WritablePlatformRegister<ICSRFlags, ICSR, true> {
	private:
		typedef ICSR this_type;

	public:
		REGISTER_BIT_RW(nmi_pending)
		REGISTER_BIT_RW(pendsv_pending)
		REGISTER_BIT_C(pendsv_clear)
		REGISTER_BIT_RW(systick_pending)
		REGISTER_BIT_C(systick_clear)
		REGISTER_BIT_R(isr_preempt)
		REGISTER_BIT_R(isr_pending)
		REGISTER_INT_R(vect_pending, 20, 12)
		REGISTER_BIT_R(ret_to_base)
		REGISTER_INT_R(vect_active, 8, 0)
};

static_assert(std::is_standard_layout<ICSR>::value, "");
static_assert(sizeof(ICSR) == sizeof(uint32_t), "");

extern ICSR icsr __attribute__((__weak__, __alias__(".SCS_ICSR")));




typedef void (*InterruptHandler)();

extern volatile InterruptHandler vtor[] __attribute__((__weak__, __alias__(".SCS_VTOR")));




enum class AIRCRFlags : uint32_t {
	big_endian = 1 << 15,
	request_system_reset = 1 << 2,
	vector_clear_active = 1 << 1,
	local_reset = 1 << 0
};

template<bool Config = false>
class AIRCR : public ConfigurationRegister<AIRCRFlags, Config, AIRCR> {
	private:
		typedef AIRCR this_type;

	public:
		void value(uint32_t val)
		{
			this->_value = (val & 0xFFFF) | 0x05FA0000;
		}

		REGISTER_INT_R(vector_key, 31, 16)
		REGISTER_BIT_R(big_endian)
		REGISTER_INT_RW(priority_group_split, 10, 8)
		REGISTER_BIT_RW(request_system_reset)
		REGISTER_BIT_C(vector_clear_active)
		REGISTER_BIT_C(local_reset)
};

static_assert(std::is_standard_layout<AIRCR<>>::value, "");
static_assert(sizeof(AIRCR<>) == sizeof(uint32_t), "");

extern AIRCR<> aircr __attribute__((__weak__, __alias__(".SCS_AIRCR")));

}

#define __REGISTERS_UNDEF
#include <hw/platform_register.hpp>

#endif /* INCLUDE_HW_INTERRUPT__HPP_ */
