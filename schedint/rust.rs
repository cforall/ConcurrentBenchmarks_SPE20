use std::env;
use std::process;
use std::thread;
use std::sync::{Arc, Mutex, Condvar};
use std::time::Instant;

fn main() {
	let mut times : u32 = 500000;
	let args: Vec<String> = env::args().collect();
	if args.len() > 2 { process::exit( 1 ); }
	if args.len() == 2 { times = args[1].parse().unwrap(); }

	let m = Arc::new(Mutex::new(0));
	let c = Arc::new(Condvar::new());

	let m2 = Arc::clone(&m);
	let c2 = Arc::clone(&c);

	let th = thread::spawn( move || {
		while *m2.lock().unwrap() == 0 { // waiter must start first
			thread::yield_now();
		}
		let start = Instant::now();
		while *m2.lock().unwrap() == 1 {
			c2.notify_one();
		}
		let duration = start.elapsed() / times;
		println!( "{:?}", duration.as_nanos() );
	});
 	{
		let mut sc = m.lock().unwrap();
		*sc = 1;
		for _ in 1..times {
			sc = c.wait(sc).unwrap();
		}
		*sc = 0;
	}
	th.join().unwrap();
}

// Local Variables: //
// tab-width: 4 //
// compile-command: "rustc -C opt-level=3 rust.rs" //
// End: //

