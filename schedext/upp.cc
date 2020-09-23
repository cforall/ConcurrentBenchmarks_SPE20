#include <cstdio>

#include "bench.h"

_Monitor M {
public:
	void __attribute__((noinline)) call() {}
	void __attribute__((noinline)) wait() {
		for ( size_t i = 0; i < times; i++ ) {
			_Accept(call);
		}
	}
} m;

_Task T {
	void main() {
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
