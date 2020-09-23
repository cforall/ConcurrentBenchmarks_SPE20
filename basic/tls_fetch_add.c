#include <stdbool.h>
#include <stdio.h>

#include "bench.h"

// Does not do a fetch & add. It mimics the cfa protocol to disable interrupts locally, by writing true or false to a
// thread_local Boolean. This means the entire protocol is just to "mov" instructions making it extremely cheap.

#define thread_local _Thread_local

thread_local volatile bool value;

void __attribute__((noinline)) do_call() {
	__atomic_store_n( &value, true, __ATOMIC_RELAXED );
	__atomic_signal_fence(__ATOMIC_ACQUIRE);
	asm volatile ("");
	__atomic_store_n( &value, false, __ATOMIC_RELAXED );
	__atomic_signal_fence(__ATOMIC_RELEASE);
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
