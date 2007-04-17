#ifndef __dlmalloc_h
#define __dlmalloc_h

#ifdef __cplusplus
extern "C"
{
#endif

void* dlmalloc(size_t);
void  dlfree(void*);
void* dlcalloc(size_t, size_t);
void* dlrealloc(void*, size_t);
void* dlmemalign(size_t, size_t);
void* dlvalloc(size_t);
int dlmallopt(int, int);
size_t dlmalloc_footprint(void);
size_t dlmalloc_max_footprint(void);
size_t dlmalloc_inuse(void);

#ifdef __cplusplus
}
#endif
#endif // _h
