#ifndef INCLUDE_HW_CPUID__HPP_
#define INCLUDE_HW_CPUID__HPP_

#include <hw/platform_register.hpp>

namespace ense {

class CPUID : public PlatformRegister<void, CPUID, volatile uint32_t> {
	public:
		REGISTER_INT_R(implementer, 31, 24)
		REGISTER_INT_R(variant, 23, 20)
		REGISTER_INT_R(architecture, 19, 16)
		REGISTER_INT_R(part_number, 15, 4)
		REGISTER_INT_R(revision, 3, 0)
};

extern linker_placed_register<CPUID> cpuid __attribute__((__weak__, __alias__(".SCS_CPUID")));

}

#define __REGISTERS_UNDEF
#include <hw/platform_register.hpp>

#endif /* INCLUDE_HW_CPUID__HPP_ */
