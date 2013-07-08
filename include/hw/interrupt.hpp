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
		uint32_t value() const { return this->_value; }

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




enum class SystemHandlerName : uint32_t {
	MemManage = 4,
	BusFaul = 5,
	UsageFault = 6,
	SVcall = 11,
	DebugMonitor = 12,
	PendSV = 14,
	SysTick = 15
};

class SystemHandlerPriorities {
	private:
		volatile uint8_t priorities[12];

	public:
		uint8_t operator[](SystemHandlerName name) const
		{
			return priorities[static_cast<uint32_t>(name) - 4];
		}

		SystemHandlerPriorities& set(SystemHandlerName name, uint8_t prio)
		{
			priorities[static_cast<uint32_t>(name) - 4] = prio;
			return *this;
		}
};

static_assert(std::is_standard_layout<SystemHandlerPriorities>::value, "");
static_assert(sizeof(SystemHandlerPriorities) == 3 * sizeof(uint32_t), "");

extern SystemHandlerPriorities system_handler_priorities __attribute__((__weak__, __alias__(".SCS_SHP")));




enum class SHCSRFlags : uint32_t {
	usage_fault_enabled = 1 << 18,
	bus_fault_enabled = 1 << 17,
	mem_fault_enabled = 1 << 16,
	svcall_pending = 1 << 15,
	bus_fault_pending = 1 << 14,
	mem_fault_pending = 1 << 13,
	usage_fault_pending = 1 << 12,
	sys_tick_active = 1 << 11,
	pendsv_active = 1 << 10,
	monitor_active = 1 << 8,
	svcall_active = 1 << 7,
	usage_fault_active = 1 << 3,
	bus_fault_active = 1 << 1,
	mem_fault_active = 1 << 0
};

template<bool Config = false>
class SHCSR : public ConfigurationRegister<SHCSRFlags, Config, SHCSR> {
	private:
		typedef SHCSR this_type;

	public:
		REGISTER_BIT_RW(usage_fault_enabled)
		REGISTER_BIT_RW(bus_fault_enabled)
		REGISTER_BIT_RW(mem_fault_enabled)
		REGISTER_BIT_RW(svcall_pending)
		REGISTER_BIT_RW(bus_fault_pending)
		REGISTER_BIT_RW(mem_fault_pending)
		REGISTER_BIT_RW(usage_fault_pending)
		REGISTER_BIT_RW(sys_tick_active)
		REGISTER_BIT_RW(pendsv_active)
		REGISTER_BIT_RW(monitor_active)
		REGISTER_BIT_RW(svcall_active)
		REGISTER_BIT_RW(usage_fault_active)
		REGISTER_BIT_RW(bus_fault_active)
		REGISTER_BIT_RW(mem_fault_active)
};

static_assert(std::is_standard_layout<SHCSR<>>::value, "");
static_assert(sizeof(SHCSR<>) == sizeof(uint32_t), "");

extern AIRCR<> shcsr __attribute__((__weak__, __alias__(".SCS_SHCSR")));

}

#define __REGISTERS_UNDEF
#include <hw/platform_register.hpp>

#endif /* INCLUDE_HW_INTERRUPT__HPP_ */
