#include <stdlib.h>

#include <memalloc.hpp>

extern "C" {

void* malloc(size_t size)
{
	return ense::memalloc::allocate(size);
}

void* realloc(void* block, size_t size)
{
	return ense::memalloc::reallocate(block, size);
}

void free(void* block)
{
	ense::memalloc::free(block);
}

}
