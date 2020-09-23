#include <stdio.h>
#include <stdint.h>										// uintptr_t

#include "bench.h"

// Does a "lock xchg" on entry but a simple "mov" on exit => cheaper as 0 contention. While it has much more code, the
// bulk is never run.

#define CALIGN __attribute__(( aligned (CACHE_ALIGN) ))
#define CACHE_ALIGN 128
#define Pause() __asm__ __volatile__ ( "pause" : : : )

typedef uintptr_t TYPE;									// addressable word-size
static volatile TYPE lock __attribute__(( aligned (128) )); // Intel recommendation
static TYPE PAD CALIGN __attribute__(( unused ));		// protect further false sharing

static inline void spin_lock( volatile TYPE *lock ) {
	enum { SPIN_START = 4, SPIN_END = 64 * 1024, };
	unsigned int spin = SPIN_START;

	for ( unsigned int i = 1;; i += 1 ) {
	  if ( *lock == 0 && __atomic_test_and_set( lock, __ATOMIC_ACQUIRE ) == 0 ) break;
		for ( volatile unsigned int s = 0; s < spin; s += 1 ) Pause(); // exponential spin
		//spin += spin;									// powers of 2
		if ( i % 64 == 0 ) spin += spin;				// slowly increase by powers of 2
		if ( spin > SPIN_END ) spin = SPIN_START;		// prevent overflow
	} // for
} // spin_lock

static inline void spin_unlock( volatile TYPE *lock ) {
	__atomic_clear( lock, __ATOMIC_RELEASE );
} // spin_unlock

void __attribute__((noinline)) do_call() {
	spin_lock( &lock );
//	asm volatile ("");
	spin_unlock( &lock );
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
