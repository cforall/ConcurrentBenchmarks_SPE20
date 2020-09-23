package main

import (
	"fmt"
	"time"
	"os"
	"strconv"
	"sync"
)

var mutex sync.Mutex

func call() {
	 mutex.Lock();
	 mutex.Unlock();
}
func main() {
	var times int = 10000000
	if len( os.Args ) > 2 { os.Exit( 1 ) }
	if len( os.Args ) == 2 { times, _ = strconv.Atoi(os.Args[1]) }

	start := time.Now()
	for i := 1; i <= times; i += 1 {
		call();
	}
	end := time.Now()
	fmt.Printf( "%d\n", end.Sub(start) / time.Duration(times) )
}

// Local Variables: //
// tab-width: 4 //
// End: //
