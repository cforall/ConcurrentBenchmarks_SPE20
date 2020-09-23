#![warn(rust_2018_idioms)]

use tokio::task;

use std::env;
use std::process;
use std::time::Instant;

#[tokio::main]
async fn main() {
	let mut times : u32 = 50000000;
	let args: Vec<String> = env::args().collect();
	if args.len() > 2 { process::exit( 1 ); }
	if args.len() == 2 { times = args[1].parse().unwrap(); }

	let start = Instant::now();
	for _ in 1..times {
		let _ : task::JoinHandle<_> = task::spawn(async {
			// some work here
		});
	}
	let duration = start.elapsed() / times;
	println!( "{:?}", duration.as_nanos() )
}

// https://docs.rs/tokio/0.2.2/tokio/task/struct.JoinHandle.html

// ~/software/rust/tokio/src/main.rs
// ~/software/rust/tokio/target/release/tokio
// $ cargo build --release
// % tokio

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// End: //
