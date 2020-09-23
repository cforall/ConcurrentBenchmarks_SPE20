#include <pthread.h>
#include <stdio.h>

#include "bench.h"

volatile int go = 0;

pthread_mutex_t m;
pthread_cond_t c;

void __attribute__((noinline)) call() {
	pthread_mutex_lock( &m );
	pthread_cond_signal( &c );
	pthread_mutex_unlock( &m );
}

void __attribute__((noinline)) wait() {
	pthread_mutex_lock(&m);
	go = 1;
	for ( size_t i = 0; i < times; i++ ) {
		pthread_cond_wait( &c, &m );
	}
	go = 0;
	pthread_mutex_unlock( &m );
}

void * thread_main( __attribute__((unused)) void * arg ) {
	while ( go == 0 ) { sched_yield(); } // waiter must start first
	// barging for lock acquire => may not execute N times
	BENCH(
		while ( go == 1 ) { call(); },
		result
	)
	printf( "%g\n", result );
	return NULL;
}

int main( int argc, char * argv[] ) {
	BENCH_START()
	pthread_t thread;
	if ( pthread_create( &thread, NULL, thread_main, NULL ) < 0 ) {
		perror( "failure" );
		return 1;
	}
	wait();
	if ( pthread_join( thread, NULL ) < 0 ) {
		perror( "failure" );
		return 1;
	}
}

// Local Variables: //
// tab-width: 4 //
// End: //
