public class JavaThread {
	// Simplistic low-quality Marsaglia Shift-XOR pseudo-random number generator.
	// Bijective
	// Cycle length for non-zero values is 4G-1.
	// 0 is absorbing and should be avoided -- fixed point.
	// The returned value is typically masked to produce a positive value.
	static volatile int Ticket = 0 ;

	private static int nextRandom (int x) {
		if (x == 0) {
			// reseed the PRNG
			// Ticket is accessed infrequently and does not constitute a coherence hot-spot.
			// Note that we use a non-atomic racy increment -- the race is rare and benign.
			// If the race is a concern switch to an AtomicInteger.
			// In addition accesses to the RW volatile global "Ticket"  variable are not
			// (readily) predictable at compile-time so the JIT will not be able to elide
			// nextRandom() invocations.
			x = ++Ticket ;
			if (x == 0) x = 1 ;
		}
		x ^= x << 6;
		x ^= x >>> 21;
		x ^= x << 7;
		return x ;
	}
	static int x = 2;

	static private long times = Long.parseLong("100000");

	public static void helper() {
		for(long i = 1; i <= times; i += 1) {
			Thread.yield();
		}
	}
	public static void InnerMain() {
		long start = System.nanoTime();
		helper();
		long end = System.nanoTime();
		System.out.println( (end - start) / times );
	}
	public static void main(String[] args) throws InterruptedException {
		if ( args.length > 1 ) System.exit( 1 );
		if ( args.length == 1 ) { times = Long.parseLong(args[0]); }

		for (int i = Integer.parseInt("5"); --i >= 0 ; ) {
			InnerMain();
			Thread.sleep(2000);	// 2 seconds
			x = nextRandom(x);
		}
		if ( x == 0 ) System.out.println(x);
	}
}

// Local Variables: //
// tab-width: 4 //
// End: //
