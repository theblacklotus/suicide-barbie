#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pspkernel.h>
#include "dlmalloc.h"

size_t allocated_memory = 0;
void* gVolatileMem = 0;
int gVolatileMemSize = 0;
int outofmem = 0;

static SceUID mutex = -1;
class ScopedMutex
{
public:
	ScopedMutex(SceUID& mutex)
	{
		if (mutex < 0)
		{
			mutex = sceKernelCreateSema("malloc_sema", 0, 1, 1, 0);
		}
		sceKernelWaitSema(mutex, 1, 0);
	}
	~ScopedMutex()
	{
		sceKernelSignalSema(mutex, 1);
	}
};

extern "C"
{

// Use 'noinline' here as we don't want it to unroll into __wrap_calloc below
void* __attribute__ ((noinline)) __wrap_malloc(size_t size)
{
	ScopedMutex scoped(mutex);
	return dlmalloc(size);
	size += 16;
	int* ptr = (int*)_malloc_r(0, size);
	if (!ptr)
	{
		printf("out-of-mem trying to get %i bytes (%i); will use volatile mem\n",size, allocated_memory);
		char* mem = (char*)gVolatileMem;
		mem += (size + 16) & ~0xf;
		ptr = (int*)gVolatileMem;
		gVolatileMem = mem;
		printf("ptr = %x, mem = %x\n", ptr, mem);
		outofmem = 1;
//		return 0;
	}
	allocated_memory += size;
	*ptr = size;
	ptr += 4;
//	printf("alloc'd %i bytes @ %x\n", size, ptr);
	return ptr;
}

void __wrap_free(void* size)
{
	ScopedMutex scoped(mutex);
	dlfree(size);
	return;
	int* ptr = (int*)size;
	if (!ptr || outofmem)
		return;
	ptr -=4;
	allocated_memory -= *ptr;
//	printf("free'd %i bytes @ %x\n", -1, ptr);
	_free_r(0, ptr);
}

void* __wrap_calloc(size_t numelems, size_t sizeofeach)
{
	void* p = __wrap_malloc(numelems * sizeofeach);
	if (!p)
		return 0;
	memset(p, 0x00, numelems * sizeofeach);
	return p;
}

}
