#include <new.h>

#ifdef ZPU

#include <inttypes.h>
#include <new>


extern "C" {
    extern void *__end__;
    static void *alloc_buffer = &__end__;
	void * malloc(int size)
	{
		void *ret = alloc_buffer;
		/* Align */
		alloc_buffer = (void*)((unsigned)alloc_buffer + size);
        alloc_buffer = (void*)(((unsigned)alloc_buffer + 3) & ~3);
		return ret;
	}
	void free(void*)
	{
	}
};

void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
} 

#else
void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
} 

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {}; 

void __cxa_pure_virtual(void) {};
#endif

