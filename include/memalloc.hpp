#ifndef INCLUDE_MEMALLOC__HPP_A5BBDC0BBB2AA0F8
#define INCLUDE_MEMALLOC__HPP_A5BBDC0BBB2AA0F8

#include <cstdint>
#include <cstring>
#include <new>

namespace ense {

class MemallocContext {
	private:
		friend struct MemallocInit;

		char* _start;
		uint32_t _size;

		void init();

	public:
		MemallocContext() = default;
		MemallocContext(char* start, uint32_t size);

		const char* start() const { return _start; }
		uint32_t size() const { return _size; }

		void* alloc(uint32_t size);
		void* resize(void* block, uint32_t newSize);
		void free(void* block);

		bool contains(const void* ptr) const
		{
			uintptr_t ustart = reinterpret_cast<uintptr_t>(_start);
			uintptr_t uptr = reinterpret_cast<uintptr_t>(ptr);

			return uptr >= ustart && uptr < ustart + _size;
		}
};



enum MemallocFlags : uint32_t {
	dmaCapable  = 1 << 0,
	coreCoupled = 1 << 1,
};



namespace detail {

extern "C" MemallocContext _memalloc_regions[];

template<unsigned Id, MemallocFlags Flags = MemallocFlags(0)>
struct MemallocInstance {
	static constexpr unsigned id = Id;
	static constexpr MemallocFlags flags = Flags;
};

template<typename...>
struct MemallocInstances;

template<unsigned... Ids, MemallocFlags... Flags>
struct MemallocInstances<MemallocInstance<Ids, Flags>...> {
	private:
		template<typename Ctx, typename... Rest>
		static void* alloc(uint32_t size, MemallocFlags wanted, Ctx ctx, Rest... rest)
		{
			void* result = alloc(size, wanted, ctx);
			return result
				? result
				: alloc(size, wanted, rest...);
		}

		template<typename Ctx>
		static void* alloc(uint32_t size, MemallocFlags wanted, Ctx)
		{
			return (Ctx::flags & wanted) == wanted
				? region(Ctx::id).alloc(size)
				: nullptr;
		}

		static void* allocateAny(uint32_t size, MemallocFlags wanted)
		{
			return alloc(size, wanted, MemallocInstance<Ids, Flags>()...);
		}

		static MemallocContext* resolveContext(void*)
		{
			return nullptr;
		}

		template<typename Ctx, typename... Rest>
		static MemallocContext* resolveContext(void* block, Ctx, Rest... rest)
		{
			if (region(Ctx::id).contains(block))
				return &region(Ctx::id);
			else
				return resolveContext(block, rest...);
		}

		static MemallocContext* resolve(void* block)
		{
			return resolveContext(block, MemallocInstance<Ids, Flags>()...);
		}

		MemallocInstances() = default;

	public:
		static void* allocate(uint32_t size, MemallocFlags wanted = MemallocFlags(0));
		static void* reallocate(void* block, uint32_t size, MemallocFlags wanted = MemallocFlags(0));
		static void free(void* block);

		static constexpr unsigned size = sizeof...(Ids);

		static MemallocContext& region(unsigned id)
		{
			return _memalloc_regions[id];
		}

		MemallocContext* begin() const { return &region(0); }
		MemallocContext* end() const { return &region(size); }

		static MemallocInstances regions()
		{
			return MemallocInstances();
		}
};

}

}

#include <hw/__memalloc.hpp>

void* operator new(size_t size, ense::MemallocFlags flags, const std::nothrow_t&) noexcept;
void* operator new(size_t size, ense::MemallocFlags flags);
void* operator new[](size_t size, ense::MemallocFlags flags, const std::nothrow_t&) noexcept;
void* operator new[](size_t size, ense::MemallocFlags flags);

void operator delete(void* block, ense::MemallocFlags, const std::nothrow_t&) noexcept;
void operator delete(void* block, ense::MemallocFlags) noexcept;
void operator delete[](void* block, ense::MemallocFlags, const std::nothrow_t&) noexcept;
void operator delete[](void* block, ense::MemallocFlags) noexcept;

#endif /* INCLUDE_MEMALLOC__HPP_A5BBDC0BBB2AA0F8 */
