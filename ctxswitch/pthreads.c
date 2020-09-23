#include <stdio.h>
#include <stdlib.h>

#include <sched.h>

#include "bench.h"

int main( int argc, char * argv[] ) {
	BENCH_START()
	BENCH(
		for (size_t i = 0; i < times; i++) {
			sched_yield();
		},
		result
	)
	printf( "%g\n", result );
}
