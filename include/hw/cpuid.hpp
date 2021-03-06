#ifndef INCLUDE_HW_CPUID__HPP_AEA6A64CBB0389A6
#define INCLUDE_HW_CPUID__HPP_AEA6A64CBB0389A6

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {

struct CPUID : PlatformRegister<CPUID, volatile uint32_t> {
	REGISTER_INT_R(implementer,  range<31, 24>)
	REGISTER_INT_R(variant,      range<23, 20>)
	REGISTER_INT_R(architecture, range<19, 16>)
	REGISTER_INT_R(part_number,  range<15, 4>)
	REGISTER_INT_R(revision,     range<3, 0>)
};

static linker_placed_register<CPUID> cpuid [[gnu::weakref(".SCS_CPUID")]];

}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_CPUID__HPP_AEA6A64CBB0389A6 */
