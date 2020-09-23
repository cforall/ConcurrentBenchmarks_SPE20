use std::env;
use std::process;
use std::time::Instant;

async fn nop() {}

#[async_std::main]
async fn main() {
	let mut times : u32 = 50000000;
	let args: Vec<String> = env::args().collect();
	if args.len() > 2 { process::exit( 1 ); }
	if args.len() == 2 { times = args[1].parse().unwrap(); }

	let start = Instant::now();
	for _ in 1..times {
		nop().await;
	}
	let duration = start.elapsed() / times;
	println!( "{:?}", duration.as_nanos() )
}

// ~/software/rust/async-std/src/main.rs
// ~/software/rust/async-std/target/release/async-std
// $ cargo build --release
// % async-std

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// End: //
