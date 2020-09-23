#include <cstdio>

#include "bench.h"

_Task MyThread {
	void main() {}
};

int main( int argc, char * argv[] ) {
	BENCH_START()
	BENCH(
		for (size_t i = 0; i < times; i++) {
			MyThread m;
		},
		result
	)
	printf( "%g\n", result );
}

// Local Variables: //
// tab-width: 4 //
// End: //
