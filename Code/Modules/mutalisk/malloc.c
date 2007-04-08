#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

size_t allocated_memory = 0;
void* __wrap_malloc(size_t size)
{
	size += 16;
	int* ptr = _malloc_r(0, size);
	if (!ptr)
	{
		printf("out-of-mem trying to get %i bytes\n",size);
		return 0;
	}
	allocated_memory += size;
	*ptr = size;
	ptr += 4;
//	printf("alloc'd %i bytes @ %x\n", size, ptr);
	return ptr;
}

void __wrap_free(void* size)
{
	int* ptr = (int*)size;
	if (!ptr)
		return;
	ptr -=4;
	allocated_memory -= *ptr;
//	printf("free'd %i bytes @ %x\n", -1, ptr);
	_free_r(0, ptr);
}

void* __wrap_calloc(size_t numelems, size_t sizeofeach)
{
	void* p = malloc(numelems * sizeofeach);
	if (!p)
		return 0;
	memset(p, 0x00, numelems * sizeofeach);
	return p;
}
