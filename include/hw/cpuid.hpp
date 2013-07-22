#ifndef INCLUDE_HW_CPUID__HPP_
#define INCLUDE_HW_CPUID__HPP_

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {

struct CPUID : PlatformRegister<void, CPUID, volatile uint32_t> {
	REGISTER_INT_R(implementer,  bit::range<31, 24>)
	REGISTER_INT_R(variant,      bit::range<23, 20>)
	REGISTER_INT_R(architecture, bit::range<19, 16>)
	REGISTER_INT_R(part_number,  bit::range<15, 4>)
	REGISTER_INT_R(revision,     bit::range<3, 0>)
};

extern linker_placed_register<CPUID> cpuid __attribute__((__weak__, __alias__(".SCS_CPUID")));

}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_CPUID__HPP_ */
