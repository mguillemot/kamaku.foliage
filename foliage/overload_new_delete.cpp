#include <new>
#include "memory.hpp"

#ifdef __PPC__

Foliage::PoolAllocator<Uint8, Foliage::EternalPool> eternalAlloc;
Foliage::PoolAllocator<Uint8, Foliage::LevelPool> levelAlloc;

#endif

// For performance considerations, our pool allocators never throw std::bad_alloc
// like they should

// Normal new/delete operators

#ifdef __PPC__

void* operator new(std::size_t sz) throw ()
{
	return (void *)eternalAlloc.allocate(sz);
}

void operator delete(void* m) throw ()
{
}

void* operator new[](std::size_t sz) throw ()
{
	return (void *)eternalAlloc.allocate(sz);
}

void operator delete[](void* m) throw ()
{
}

#endif

// Pool-specific new/delete operators

void* operator new(std::size_t sz, const Foliage::MemoryPersistance pool) throw ()
{
	#ifdef __PPC__
		switch (pool)
		{
			case Foliage::Level:
				return (void *)levelAlloc.allocate(sz);
			default:
				return (void *)eternalAlloc.allocate(sz);
		}
	#else
		return operator new(sz);
	#endif
}

void operator delete(void* m, const Foliage::MemoryPersistance pool) throw ()
{
	#ifndef __PPC__
		delete m;
	#endif
}

void* operator new[](std::size_t sz, const Foliage::MemoryPersistance pool) throw ()
{
	#ifdef __PPC__
		switch (pool)
		{
			case Foliage::Level:
				return (void *)levelAlloc.allocate(sz);
			default:
				return (void *)eternalAlloc.allocate(sz);
		}
	#else
		return operator new(sz);
	#endif
}

void operator delete[](void* m, const Foliage::MemoryPersistance pool) throw ()
{
	#ifndef __PPC__
		delete m;
	#endif
}

// No-throw new/delete operators (for STL compatibility)

#ifdef __PPC__

void* operator new(std::size_t sz, const std::nothrow_t&) throw ()
{
	return (void *)eternalAlloc.allocate(sz);
}

void operator delete(void* m, const std::nothrow_t&) throw ()
{
}

void* operator new[](std::size_t sz, const std::nothrow_t&) throw ()
{
	return (void *)eternalAlloc.allocate(sz);
}

void operator delete[](void* m, const std::nothrow_t&) throw ()
{
}

#endif
