class Monitor {
	public static volatile Boolean go = false;
	public static volatile Boolean next = false;
}

class Signaller extends Thread {
	Monitor m;
	Signaller(Monitor m) {
		this.m = m;
	}

	public void run() {
		Monitor.go = true;
		while( Monitor.go ) {
			synchronized(this.m) {
				Monitor.next = false;
				this.m.notify();
			}
			while( ! Monitor.next && Monitor.go );	// spin until woken
		}
	}
}

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

	static private long times = Long.parseLong("1000000");

	public static void helper( Monitor m ) throws InterruptedException {
		for(long i = 1; i <= times; i += 1) {
			m.wait();		// relase monitor lock
			m.next = true;
		}
	}
	public static void InnerMain() throws InterruptedException {
		Monitor m = new Monitor();
		long start, end;
		Signaller s = new Signaller(m);
		synchronized(m) {
			s.start();
			while( ! Monitor.go ) { // waiter must start first
				Thread.yield();
			}
			start = System.nanoTime();
			helper( m );
			end = System.nanoTime();
		}
		Monitor.go = false;
		s.join();
		System.out.println( (end - start) / times);
	}
	public static void main(String[] args) throws InterruptedException {
		if ( args.length > 2 ) System.exit( 1 );
		if ( args.length == 2 ) { times = Long.parseLong(args[1]); }

		for (int n = Integer.parseInt("5"); --n >= 0 ; ) {
			InnerMain();
			Thread.sleep(2000);     // 2 seconds
			x = nextRandom(x);
		}
		if ( x == 0 ) System.out.println(x);
	}
}

// Local Variables: //
// tab-width: 4 //
// End: //
