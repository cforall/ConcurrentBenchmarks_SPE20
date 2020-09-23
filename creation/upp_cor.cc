#include <cstdio>

#include "bench.h"

_Coroutine MyCor {
	void main() {}
  public:
	MyCor() { resume(); }
};

int main( int argc, char * argv[] ) {
	BENCH_START()
	BENCH(
		for (size_t i = 0; i < times; i++) {
			MyCor m;
		},
		result
	)
	printf( "%g\n", result );
}

// Local Variables: //
// tab-width: 4 //
// End: //
