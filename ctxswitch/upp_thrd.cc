#include <cstdio>

#include "bench.h"

int main( int argc, char * argv[] ) {
	BENCH_START()
	BENCH(
		for (size_t i = 0; i < times; i++) {
			uThisTask().yield();
		},
		result
	)
	printf( "%g\n", result );
}

// Local Variables: //
// tab-width: 4 //
// End: //
