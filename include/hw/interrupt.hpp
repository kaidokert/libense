#ifndef INCLUDE_HW_INTERRUPT__HPP_
#define INCLUDE_HW_INTERRUPT__HPP_

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {

class ICTR : public PlatformRegister<void, ICTR, volatile uint32_t> {
	public:
		uint32_t lines() const
		{
			uint32_t line_code = _value & 0xF;
			return line_code == 0xf ? 496 : 32 * (line_code + 1);
		}
};

extern linker_placed_register<ICTR> ictr __attribute__((__weak__, __alias__(".SCS_ICTR")));




extern volatile uint32_t actlr __attribute__((__weak__, __alias__(".SCS_ACTLR")));




inline void trigger_interrupt(uint32_t num)
{
	extern volatile uint32_t stir __attribute__((__weak__, __alias__(".SCS_STIR")));
	stir = num & 0x1FF;
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

class ICSR : public WritablePlatformRegister<ICSRFlags, ICSR, volatile uint32_t> {
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

extern linker_placed_register<ICSR> icsr __attribute__((__weak__, __alias__(".SCS_ICSR")));




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

extern linker_placed_register<AIRCR<>> aircr __attribute__((__weak__, __alias__(".SCS_AIRCR")));




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
	public:
		typedef volatile uint8_t value_type[12];

	private:
		value_type priorities;

	public:
		uint8_t operator[](SystemHandlerName name) const
		{
			return priorities[static_cast<uint32_t>(name) - 4];
		}

		volatile uint8_t& operator[](SystemHandlerName name)
		{
			return priorities[static_cast<uint32_t>(name) - 4];
		}

		SystemHandlerPriorities& set(SystemHandlerName name, uint8_t prio)
		{
			priorities[static_cast<uint32_t>(name) - 4] = prio;
			return *this;
		}
};

extern linker_placed_array<SystemHandlerPriorities> system_handler_priorities __attribute__((__weak__, __alias__(".SCS_SHP")));




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

extern linker_placed_register<SHCSR<>> shcsr __attribute__((__weak__, __alias__(".SCS_SHCSR")));




enum class MMFSRFlags : uint8_t {
	mmar_valid = 1 << 7,
	mlsp_err = 1 << 5,
	mstack_err = 1 << 4,
	munstack_err = 1 << 3,
	dacc_violation = 1 << 1,
	iacc_violation = 1 << 0,

	clear = 1
};

enum class BFSRFlags : uint8_t {
	bfar_valid = 1 << 7,
	lsp_err = 1 << 5,
	stack_err = 1 << 4,
	unstack_err = 1 << 3,
	imprecise_err = 1 << 2,
	precise_err = 1 << 1,
	ibus_err = 1 << 0,

	clear = 1
};

enum class UFSRFlags : uint16_t {
	div_by_zero = 1 << 9,
	unaligned = 1 << 8,
	no_coprocessor = 1 << 3,
	invalid_pc = 1 << 2,
	invalid_state = 1 << 1,
	undefined_instruction = 1 << 0,

	clear = 1
};

enum class CFSRFlags : uint32_t {
	div_by_zero = static_cast<uint32_t>(UFSRFlags::div_by_zero) << 16,
	unaligned = static_cast<uint32_t>(UFSRFlags::unaligned) << 16,
	no_coprocessor = static_cast<uint32_t>(UFSRFlags::no_coprocessor) << 16,
	invalid_pc = static_cast<uint32_t>(UFSRFlags::invalid_pc) << 16,
	invalid_state = static_cast<uint32_t>(UFSRFlags::invalid_state) << 16,
	undefined_instruction = static_cast<uint32_t>(UFSRFlags::undefined_instruction) << 16,
	clear_ufsr = static_cast<uint32_t>(UFSRFlags::undefined_instruction) << 16,

	bfar_valid = static_cast<uint32_t>(BFSRFlags::bfar_valid) << 8,
	lsp_err = static_cast<uint32_t>(BFSRFlags::lsp_err) << 8,
	stack_err = static_cast<uint32_t>(BFSRFlags::stack_err) << 8,
	unstack_err = static_cast<uint32_t>(BFSRFlags::unstack_err) << 8,
	imprecise_err = static_cast<uint32_t>(BFSRFlags::imprecise_err) << 8,
	precise_err = static_cast<uint32_t>(BFSRFlags::precise_err) << 8,
	ibus_err = static_cast<uint32_t>(BFSRFlags::ibus_err) << 8,
	clear_bfsr = static_cast<uint32_t>(BFSRFlags::ibus_err) << 8,

	mmar_valid = static_cast<uint32_t>(MMFSRFlags::mmar_valid),
	mlsp_err = static_cast<uint32_t>(MMFSRFlags::mlsp_err),
	mstack_err = static_cast<uint32_t>(MMFSRFlags::mstack_err),
	munstack_err = static_cast<uint32_t>(MMFSRFlags::munstack_err),
	dacc_violation = static_cast<uint32_t>(MMFSRFlags::dacc_violation),
	iacc_violation = static_cast<uint32_t>(MMFSRFlags::iacc_violation),
	clear_mmfsr = static_cast<uint32_t>(MMFSRFlags::iacc_violation)
};

template<bool Config = false>
class CFSR : public ConfigurationRegister<CFSRFlags, Config, CFSR> {
	private:
		typedef CFSR this_type;

	public:
		REGISTER_BIT_R(div_by_zero)
		REGISTER_BIT_R(unaligned)
		REGISTER_BIT_R(no_coprocessor)
		REGISTER_BIT_R(invalid_pc)
		REGISTER_BIT_R(invalid_state)
		REGISTER_BIT_R(undefined_instruction)
		REGISTER_BIT_C(clear_ufsr)

		REGISTER_BIT_R(bfar_valid)
		REGISTER_BIT_R(lsp_err)
		REGISTER_BIT_R(stack_err)
		REGISTER_BIT_R(unstack_err)
		REGISTER_BIT_R(imprecise_err)
		REGISTER_BIT_R(precise_err)
		REGISTER_BIT_R(ibus_err)
		REGISTER_BIT_C(clear_bfsr)

		REGISTER_BIT_R(mmar_valid)
		REGISTER_BIT_R(mlsp_err)
		REGISTER_BIT_R(mstack_err)
		REGISTER_BIT_R(munstack_err)
		REGISTER_BIT_R(dacc_violation)
		REGISTER_BIT_R(iacc_violation)
		REGISTER_BIT_C(clear_mmfsr)
};

class MMFSR : public WritablePlatformRegister<MMFSRFlags, MMFSR, volatile uint8_t> {
	private:
		typedef MMFSR this_type;

	public:
		REGISTER_BIT_R(mmar_valid)
		REGISTER_BIT_R(mlsp_err)
		REGISTER_BIT_R(mstack_err)
		REGISTER_BIT_R(munstack_err)
		REGISTER_BIT_R(dacc_violation)
		REGISTER_BIT_R(iacc_violation)
		REGISTER_BIT_C(clear)
};

class BFSR : public WritablePlatformRegister<BFSRFlags, BFSR, volatile uint8_t> {
	private:
		typedef BFSR this_type;

	public:
		REGISTER_BIT_R(bfar_valid)
		REGISTER_BIT_R(lsp_err)
		REGISTER_BIT_R(stack_err)
		REGISTER_BIT_R(unstack_err)
		REGISTER_BIT_R(imprecise_err)
		REGISTER_BIT_R(precise_err)
		REGISTER_BIT_R(ibus_err)
		REGISTER_BIT_C(clear)
};

class UFSR : public WritablePlatformRegister<UFSRFlags, UFSR, volatile uint16_t> {
	private:
		typedef UFSR this_type;

	public:
		REGISTER_BIT_R(div_by_zero)
		REGISTER_BIT_R(unaligned)
		REGISTER_BIT_R(no_coprocessor)
		REGISTER_BIT_R(invalid_pc)
		REGISTER_BIT_R(invalid_state)
		REGISTER_BIT_R(undefined_instruction)
		REGISTER_BIT_C(clear)
};

extern linker_placed_register<CFSR<>> cfsr __attribute__((__weak__, __alias__(".SCS_CFSR")));
extern linker_placed_register<MMFSR> mmfsr __attribute__((__weak__, __alias__(".SCS_MMFSR")));
extern linker_placed_register<BFSR> bfsr __attribute__((__weak__, __alias__(".SCS_BFSR")));
extern linker_placed_register<UFSR> ufsr __attribute__((__weak__, __alias__(".SCS_UFSR")));




enum class HFSRFlags : uint32_t {
	debug_event = 1U << 31,
	forced = 1 << 30,
	vector_table_fault = 1 << 1,

	clear = 1 << 1
};

class HFSR : public WritablePlatformRegister<HFSRFlags, HFSR, volatile uint32_t> {
	private:
		typedef HFSR this_type;

	public:
		REGISTER_BIT_R(debug_event)
		REGISTER_BIT_R(forced)
		REGISTER_BIT_R(vector_table_fault)
		REGISTER_BIT_C(clear)
};

extern linker_placed_register<HFSR> hfsr __attribute__((__weak__, __alias__(".SCS_HFSR")));




extern volatile void* const mmfar __attribute__((__weak__, __alias__(".SCS_MMFAR")));

extern volatile void* const bfar __attribute__((__weak__, __alias__(".SCS_BFAR")));




extern uint32_t afsr __attribute__((__weak__, __alias__(".SCS_AFSR")));

}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_INTERRUPT__HPP_ */
