#include <pthread.h>
#include <stdio.h>

#include "bench.h"

static void * foo(void *arg) {
    return arg;
}

int main( int argc, char * argv[] ) {
	BENCH_START()
	BENCH(
		for (size_t i = 0; i < times; i++) {
			pthread_t thread;
			if (pthread_create(&thread, NULL, foo, NULL) < 0) {
				perror( "failure" );
				return 1;
			}
			if (pthread_join( thread, NULL) < 0) {
				perror( "failure" );
				return 1;
			}
		},
		result
	)
	printf( "%g\n", result );
}

// Local Variables: //
// tab-width: 4 //
// End: //
