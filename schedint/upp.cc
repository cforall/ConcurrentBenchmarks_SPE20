#include <cstdio>

#include "bench.h"

volatile int go = 0;

_Monitor M {
	uCondition cond;
public:
	void __attribute__((noinline)) call() {
		cond.signal();
	}
	void __attribute__((noinline)) wait() {
		go = 1;
		for ( size_t i = 0; i < times; i++ ) {
			cond.wait();
		}
	}
} m;

_Task T {
	void main() {
		while ( go == 0 ) { yield(); } // waiter must start first
		BENCH(
			for ( size_t i = 0; i < times; i++ ) {
				m.call();
			},
			result
		)
		printf( "%g\n", result );
	}
};

int main( int argc, char * argv[] ) {
	BENCH_START()
	T t;
	m.wait();
}

// Local Variables: //
// tab-width: 4 //
// End: //
