#include <memalloc.hpp>

#include <cstddef>
#include <cstring>
#include <type_traits>
#include <utility>

namespace {

uintptr_t alignTo(uintptr_t addr, unsigned size)
{
	unsigned alignMask = size - 1;

	return (addr & alignMask) == 0
		? addr
		: (addr | alignMask) + 1;
}

template<typename T = char>
T* alignTo(void* ptr, unsigned size = alignof(T))
{
	return reinterpret_cast<T*>(alignTo(reinterpret_cast<uintptr_t>(ptr), size));
}

uint32_t ptrDiff(const void* a, const void* b)
{
	return reinterpret_cast<uintptr_t>(a) - reinterpret_cast<uintptr_t>(b);
}



struct BlockHeader {
	union {
		BlockHeader* next;
		ense::MemallocContext* owner;
	};
	uint32_t size;

	char* data()
	{
		return reinterpret_cast<char*>(this) + sizeof(BlockHeader);
	}

	char* dataEnd()
	{
		return data() + size;
	}

	static BlockHeader* before(void* data)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(data);

		return reinterpret_cast<BlockHeader*>(addr - sizeof(BlockHeader));
	}

	static BlockHeader* after(void* data)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(data);
		uintptr_t alignedDataAddr =
			alignTo(addr + sizeof(BlockHeader), alignof(std::max_align_t));

		return reinterpret_cast<BlockHeader*>(alignedDataAddr - sizeof(BlockHeader));
	}

	static bool fitsAfter(void* data, void* end)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(data);
		uintptr_t limit = reinterpret_cast<uintptr_t>(end);
		uintptr_t alignedDataAddr =
			alignTo(addr + sizeof(BlockHeader), alignof(std::max_align_t));

		return alignedDataAddr > addr && alignedDataAddr <= limit;
	}
};

static_assert(sizeof(BlockHeader) % alignof(BlockHeader) == 0, "");
static_assert(alignof(BlockHeader) <= alignof(std::max_align_t), "");
static_assert(std::is_standard_layout<BlockHeader>::value, "");



class MemallocRegion {
	private:
		BlockHeader* free;

		BlockHeader* takeBestBlock(uint32_t size);
		void maybeSplitAllocated(BlockHeader* block, uint32_t size);
		std::pair<BlockHeader*, BlockHeader*> takeFreeSurroundings(BlockHeader* block);

	public:
		void init(uint32_t size);

		void* allocate(uint32_t size, ense::MemallocContext* owner);
		void* resize(void* data, uint32_t newSize);
		void deallocate(void* data);
};

static_assert(std::is_standard_layout<MemallocRegion>::value, "");

BlockHeader* MemallocRegion::takeBestBlock(uint32_t size)
{
	struct {
		BlockHeader* block;
		BlockHeader** ptr;
	} result{};

	auto perfectMatch = [=] (const BlockHeader* bh) { return bh && bh->size == size; };

	for (auto* block = free, **ptrBlock = &free;
			block && !perfectMatch(result.block);
			ptrBlock = &block->next, block = block->next) {
		if (block->size >= size &&
				(!result.block || result.block->size > block->size))
			result = { block, ptrBlock };
	}

	if (result.block)
		*result.ptr = result.block->next;

	return result.block;
}

void MemallocRegion::maybeSplitAllocated(BlockHeader* block, uint32_t size)
{
	char* firstUnusedAddr = block->data() + size;

	if (!BlockHeader::fitsAfter(firstUnusedAddr, block->dataEnd()))
		return;

	BlockHeader* nextBlock = BlockHeader::after(firstUnusedAddr);
	uint32_t nextBlockSize = ptrDiff(block->dataEnd(), nextBlock->data());
	uint32_t oldBlockSize = ptrDiff(nextBlock, block->data());

	block->size = oldBlockSize;
	*nextBlock = { { free }, nextBlockSize };
	free = nextBlock;
}

std::pair<BlockHeader*, BlockHeader*> MemallocRegion::takeFreeSurroundings(BlockHeader* block)
{
	struct {
		BlockHeader* block;
		BlockHeader** ptr;
	} preceding{}, following{};

	for (auto* entry = free, **ptrEntry = &free;
			entry && !(preceding.block && following.block);
			ptrEntry = &entry->next, entry = entry->next) {
		if (entry->dataEnd() == reinterpret_cast<char*>(block))
			preceding = { entry, ptrEntry };
		if (block->dataEnd() == reinterpret_cast<char*>(entry))
			following = { entry, ptrEntry };
	}

	if (preceding.block && following.block) {
		if (preceding.block->next == following.block) {
			*preceding.ptr = following.block->next;
			return { preceding.block, following.block };
		}
		if (following.block->next == preceding.block) {
			*following.ptr = preceding.block->next;
			return { preceding.block, following.block };
		}
	}

	if (preceding.block)
		*preceding.ptr = preceding.block->next;
	if (following.block)
		*following.ptr = following.block->next;

	return { preceding.block, following.block };
}

void MemallocRegion::init(uint32_t size)
{
	free = BlockHeader::after(reinterpret_cast<char*>(this) + sizeof(*this));

	char* regionBegin = free->data();
	char* regionEnd = reinterpret_cast<char*>(this) + size;
	uint32_t usableSpace = ptrDiff(regionEnd, regionBegin);

	*free = { { nullptr }, usableSpace };
}

void* MemallocRegion::allocate(uint32_t size, ense::MemallocContext* owner)
{
	BlockHeader* result = takeBestBlock(size);

	if (!result)
		return nullptr;

	maybeSplitAllocated(result, size);
	result->owner = owner;
	return result->data();
}

void* MemallocRegion::resize(void* data, uint32_t newSize)
{
	BlockHeader* block = BlockHeader::before(data);
	uint32_t contentSize = block->size > newSize ? block->size : newSize;

	auto borders = takeFreeSurroundings(block);
	BlockHeader* preceding = borders.first;
	BlockHeader* following = borders.second;

	uint32_t available =
		(preceding ? preceding->size + sizeof(BlockHeader) : 0) +
		(following ? following->size + sizeof(BlockHeader) : 0);

	if (block->size + available < newSize)
		return nullptr;

	if (following)
		block->size += sizeof(BlockHeader) + following->size;

	if (preceding) {
		preceding->owner = block->owner;
		preceding->size += sizeof(BlockHeader) + block->size;
		memmove(preceding->data(), block->data(), contentSize);
		block = preceding;
	}

	maybeSplitAllocated(block, newSize);
	return block->data();
}

void MemallocRegion::deallocate(void* data)
{
	BlockHeader* block = BlockHeader::before(data);
	auto borders = takeFreeSurroundings(block);
	BlockHeader* preceding = borders.first;
	BlockHeader* following = borders.second;

	if (following)
		block->size += sizeof(BlockHeader) + following->size;

	if (preceding) {
		preceding->size += sizeof(BlockHeader) + block->size;
		block = preceding;
	}

	block->next = free;
	free = block;
}

}

namespace ense {

static_assert(std::is_standard_layout<MemallocContext>::value, "");
static_assert(std::is_trivially_constructible<MemallocContext>::value, "");
static_assert(alignof(MemallocContext) == 4, "");
static_assert(sizeof(MemallocContext) == 8, "");

MemallocContext::MemallocContext(void* start, uint32_t size)
	: _start(alignTo<MemallocRegion>(start)),
	  _size(size - ptrDiff(_start, start))
{
	init();
}

void MemallocContext::init()
{
	reinterpret_cast<MemallocRegion*>(_start)->init(_size);
}

void* MemallocContext::alloc(uint32_t size)
{
	return reinterpret_cast<MemallocRegion*>(_start)->allocate(size, this);
}

void* MemallocContext::resize(void* block, uint32_t size)
{
	return reinterpret_cast<MemallocRegion*>(_start)->resize(block, size);
}

void MemallocContext::free(void* block)
{
	if (block)
		reinterpret_cast<MemallocRegion*>(_start)->deallocate(block);
}

MemallocContext* MemallocContext::resolve(void* block)
{
	return BlockHeader::before(block)->owner;
}

}
