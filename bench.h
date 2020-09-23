#pragma once

#if defined(__cforall)
extern "C" {
#endif
	#include <stdlib.h>
	#include <stdint.h>				// uint64_t
	#include <unistd.h>				// sysconf
#if ! defined(__cforall)
	#include <time.h>
	#include <sys/time.h>
#else
}
#include <time.hfa>
#endif


static inline uint64_t bench_time() {
	struct timespec ts;
	clock_gettime( CLOCK_THREAD_CPUTIME_ID, &ts );
	return 1000000000LL * ts.tv_sec + ts.tv_nsec;
} // bench_time

#ifndef BENCH_N
#define BENCH_N 10000000
#endif

size_t times = BENCH_N;

#define BENCH_START()				\
	if ( argc > 2 ) exit( EXIT_FAILURE );	\
	if ( argc == 2 ) {			\
		times = atoi( argv[1] );	\
	}

#define BENCH(statement, output)		\
	uint64_t StartTime, EndTime;		\
	StartTime = bench_time();		\
	statement;				\
	EndTime = bench_time();			\
	double output = (double)( EndTime - StartTime ) / times;


#if defined(__cforall)
Duration default_preemption() {
	return 0;
}
#endif
#if defined(__U_CPLUSPLUS__)
unsigned int uDefaultPreemption() {
	return 0;
}
#endif
