#include <stdio.h>

#include "bench.h"

void __attribute__((noinline)) do_call() {
	asm volatile("" ::: "memory");
}

int main( int argc, char * argv[] ) {
	BENCH_START()
	BENCH(
		for (size_t i = 0; i < times; i++) {
			do_call();
		},
		result
	)
	printf( "%g\n", result );
}

// Local Variables: //
// tab-width: 4 //
// End: //
