use std::env;
use std::process;
use std::thread;
use std::time::Instant;

fn main() {
	let mut times : u32 = 50000000;
	let args: Vec<String> = env::args().collect();
	if args.len() > 2 { process::exit( 1 ); }
	if args.len() == 2 { times = args[1].parse().unwrap(); }

	let start = Instant::now();
	let th = thread::spawn( move || {
		for _ in 1..times {
			thread::yield_now();
		}
	});
	th.join().unwrap();
	let duration = start.elapsed() / times;
	println!( "{:?}", duration.as_nanos() )
}


// Local Variables: //
// tab-width: 4 //
// compile-command: "rustc -C opt-level=3 rust_thrd.rs" //
// End: //
