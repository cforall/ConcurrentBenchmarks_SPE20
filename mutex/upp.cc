#include <cstdio>

#include "bench.h"

_Monitor MyMonitor {
public:
	void __attribute__((noinline)) call() {}
};

int main( int argc, char * argv[] ) {
	BENCH_START()
	MyMonitor m;
	BENCH(
		for ( size_t i = 0; i < times; i++ ) {
			m.call();
		},
		result
	)
	printf( "%g\n", result );
}

// Local Variables: //
// tab-width: 4 //
// End: //
