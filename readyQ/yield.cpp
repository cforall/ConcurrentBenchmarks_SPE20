#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <climits>

extern "C" {
	#include <locale.h>
	#include <getopt.h>
}

#include <unistd.h>

#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using duration_t = std::chrono::duration<double>;
using std::chrono::nanoseconds;


template<typename Ratio, typename T>
T duration_cast(T seconds) {
	return std::chrono::duration_cast<std::chrono::duration<T, Ratio>>(std::chrono::duration<T>(seconds)).count();
}

volatile bool run = false;
volatile unsigned long long global_counter;

#include "libfibre/fibre.h"

FibreBarrier * barrier;
struct __attribute__((aligned(128))) counter_t {
	int value = 0;
};

void fibre_main( counter_t * counter ) {
	barrier->wait();
	// /* paranoid */ assert( true == __atomic_load_n(&run, __ATOMIC_RELAXED) );

	while(__atomic_load_n(&run, __ATOMIC_RELAXED)) {
		Fibre::forceYield();
		// fibre_yield();
		counter->value++;
	}
	__atomic_fetch_add(&global_counter, counter->value, __ATOMIC_SEQ_CST);
}

int main(int argc, char * argv[]) {
	double duration = 5;
	int nprocs = 1;
	int nthreads = 1;

	std::cout.imbue(std::locale(""));
	setlocale(LC_ALL, "");

	for(;;) {
		static struct option options[] = {
			{"duration",  required_argument, 0, 'd'},
			{"nprocs",    required_argument, 0, 'p'},
			{"nthreads",  required_argument, 0, 't'},
			{0, 0, 0, 0}
		};

		int idx = 0;
		int opt = getopt_long(argc, argv, "d:p:t:", options, &idx);

		const char * arg = optarg ? optarg : "";
		size_t len = 0;
		char * end;
		switch(opt) {
			case -1:
				goto run;
			// Numeric Arguments
			case 'd':
				duration = strtod(arg, &end);
				if(*end != '\0') {
					fprintf(stderr, "Duration must be a valid double, was %s\n", arg);
					goto usage;
				}
				break;
			case 't':
				nthreads = strtoul(arg, &end, 10);
				if(*end != '\0' || nthreads < 1) {
					fprintf(stderr, "Number of threads must be a positive integer, was %s\n", arg);
					goto usage;
				}
				break;
			case 'p':
				nprocs = strtoul(arg, &end, 10);
				if(*end != '\0' || nprocs < 1) {
					fprintf(stderr, "Number of processors must be a positive integer, was %s\n", arg);
					goto usage;
				}
				break;
			// Other cases
			default: /* ? */
				fprintf( stderr, "Unkown option '%c'\n", opt);
			usage:
				fprintf( stderr, "Usage: %s [options]\n", argv[0]);
				fprintf( stderr, "\n" );
				fprintf( stderr, "  -d, --duration=DURATION  Duration of the experiment, in seconds\n" );
				fprintf( stderr, "  -t, --nthreads=NTHREADS  Number of kernel threads\n" );
				fprintf( stderr, "  -q, --nqueues=NQUEUES    Number of queues per threads\n" );
				exit(1);
		}
	}
	run:

	{
		printf("Running %d threads on %d processors for %lf seconds\n", nthreads, nprocs, duration);

		FibreInit();
		barrier = new FibreBarrier(nthreads + 1);
		{
			Context::CurrCluster().addWorkers(nprocs);
			{
				counter_t counters[nthreads];
				Fibre threads[nthreads];
				for(int i = 0; i < nthreads; i++) {
					threads[i].run(fibre_main, &counters[i]);
				}
				printf("Starting\n");
				bool is_tty = isatty(STDOUT_FILENO);
				auto before = Clock::now();
				run = true;

				barrier->wait();
				for(;;) {
					usleep(500'000);
					auto now = Clock::now();
					duration_t durr = now - before;
					if( durr.count() > duration ) {
						break;
					}
					if(is_tty) {
						std::cout << "\r" << std::setprecision(4) << durr.count();
						std::cout.flush();
					}
				}

				auto after = Clock::now();
				duration_t durr = after - before;
				duration = durr.count();
				run = false;
				printf("\nDone\n");
				for(auto & thread : threads) {
					thread.join();
				}

				// for(const auto & counter : counters) {
				// 	std::cout << counter.value << std::endl;
				// }
			}
		}

		auto dur_nano = duration_cast<std::nano>(duration);

		std::cout << "Took " << duration << " s\n";
		printf("Total yields        : %'15llu\n", global_counter );
		printf("Yields per procs    : %'15llu\n", global_counter / nprocs );
		printf("Yields per second   : %'18.2lf\n", ((double)global_counter) / duration );
		printf("Yields/sec/procs    : %'18.2lf\n", (((double)global_counter) / nprocs) / duration );
		printf("ns per yields       : %'18.2lf\n", dur_nano / global_counter );
		printf("ns per yields/procs : %'18.2lf\n", dur_nano / (global_counter / nprocs) );

	}
}