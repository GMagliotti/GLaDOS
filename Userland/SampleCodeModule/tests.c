#include <tests.h>

void test0Div() {
    int var=0;

    var=80/var;
}

void testMalloc() {
	void * theptr = call_to_malloc((uint32_t)0x100);
	*((char *) theptr) = 69;
}