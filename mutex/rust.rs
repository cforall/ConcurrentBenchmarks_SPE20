use std::env;
use std::process;
use std::sync::Mutex;
use std::time::Instant;

fn call( lock : & Mutex<u32> ) {
	let _ = lock.lock();
}

fn main() {
	let mut times : u32 = 50000000;
	let args: Vec<String> = env::args().collect();
	if args.len() > 2 { process::exit( 1 ); }
	if args.len() == 2 { times = args[1].parse().unwrap(); }

	let lock = Mutex::new(0);

	let start = Instant::now();
	for _ in 1..times {
		call( &lock );
	}
	let duration = start.elapsed() / times;
	println!( "{:?}", duration.as_nanos() )
}

// Local Variables: //
// tab-width: 4 //
// compile-command: "rustc -C opt-level=3 rust.rs" //
// End: //
