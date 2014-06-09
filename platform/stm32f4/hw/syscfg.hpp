#ifndef INCLUDE_HW_STM32F4_SYSCFG__HPP_90B1D7E5BF3FC456
#define INCLUDE_HW_STM32F4_SYSCFG__HPP_90B1D7E5BF3FC456

#include <hw/config_register.hpp>
#include <hw/platform_register_macros.hpp>

namespace ense {
namespace platform {
namespace syscfg {

enum class MemorySource : uint32_t {
	main_flash   = 0,
	system_flash = 1,
	fsmc         = 2,
	sram         = 3
};

template<bool Config = false>
struct MEMRMP : ConfigurationRegister<void, Config, MEMRMP> {
	REGISTER_FIELD_RW(MemorySource, source, range<0, 1>)
};

static linker_placed_register<MEMRMP<>> memrmp [[gnu::weakref(".SYSCFG_MEMRMP")]];



enum class PMCFlags : uint32_t {
	ethernet_use_rmii = 1UL << 23
};

template<bool Config = false>
struct PMC : ConfigurationRegister<PMCFlags, Config, PMC> {
	REGISTER_BIT_RW(ethernet_use_rmii)
};

static linker_placed_register<PMC<>> pmc [[gnu::weakref(".SYSCFG_PMC")]];



enum class InterruptSource : uint32_t {
	portA = 0,
	portB = 1,
	portC = 2,
	portD = 3,
	portE = 4,
	portF = 5,
	portG = 6,
	portH = 7,
	portI = 8,
};

template<bool Config = false>
struct EXTICR : ConfigurationRegister<void, Config, EXTICR> {
	REGISTER_SINGULAR_ARRAY_RW(InterruptSource[16], width<4>, range<0, 128>, element_offsets<0, 4, 8, 12, 32, 36, 40, 44, 64, 68, 72, 76, 96, 100, 104, 108>)
};

static linker_placed_register<EXTICR<>> exticr [[gnu::weakref(".SYSCFG_EXTICR")]];



enum class CMPCRFlags : uint32_t {
	ready      = 1UL << 8,
	power_down = 1UL << 0
};

template<bool Config = false>
struct CMPCR : ConfigurationRegister<CMPCRFlags, Config, CMPCR> {
	REGISTER_BIT_R(ready)
	REGISTER_BIT_RW(power_down)
};

static linker_placed_register<CMPCR<>> cmpcr [[gnu::weakref(".SYSCFG_CMPCR")]];

}
}
}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_SYSCFG__HPP_90B1D7E5BF3FC456 */
