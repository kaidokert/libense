#ifndef PLATFORM_STM32F40_HW_MEMALLOC__HPP_6A1947E6111A4F77
#define PLATFORM_STM32F40_HW_MEMALLOC__HPP_6A1947E6111A4F77

#include <memalloc.hpp>

namespace ense {

typedef detail::MemallocInstances<
		detail::MemallocInstance<0, dmaCapable>,
		detail::MemallocInstance<1, coreCoupled>
	> memalloc;

}

#endif /* PLATFORM_STM32F40_HW_MEMALLOC__HPP_6A1947E6111A4F77 */
