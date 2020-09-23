#include <cstdio>

#include "bench.h"

_Coroutine C {
	void main() {
		while( true ) {
			suspend();
		}
	}
  public:
	void do_resume() {
		resume();
	}
} c;
int main( int argc, char * argv[] ) {
	BENCH_START()
	BENCH(
		for (size_t i = 0; i < times; i++) {
			c.do_resume();
		},
		result
	)
	printf( "%g\n", result );
}

// Local Variables: //
// tab-width: 4 //
// End: //
