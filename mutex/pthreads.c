#include <pthread.h>
#include <stdio.h>

#include "bench.h"

pthread_mutex_t mutex;

void __attribute__((noinline)) call() {
	 pthread_mutex_lock( &mutex );
	 pthread_mutex_unlock( &mutex );
}
int main( int argc, char * argv[] ) {
	BENCH_START()
	BENCH(
		for ( size_t i = 0; i < times; i++ ) {
			call();
		},
		result
	)
	printf( "%g\n", result );
}

// Local Variables: //
// tab-width: 4 //
// End: //
