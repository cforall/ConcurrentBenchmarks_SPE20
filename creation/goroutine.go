package main

import (
	"fmt"
	"time"
	"os"
	"strconv"
)

var shake chan bool = make( chan bool )

func noop() {
	shake <- true   // indicate completion
}

//=======================================
// benchmark driver
//=======================================

func main() {
	var times int = 10000000
	if len( os.Args ) > 2 { os.Exit( 1 ) }
	if len( os.Args ) == 2 { times, _ = strconv.Atoi(os.Args[1]) }

	start := time.Now()
	for i := 1; i <= times; i += 1 {
		go noop()		// creation
		<- shake		// wait for completion
	}
	end := time.Now()
	fmt.Printf( "%d\n", end.Sub(start) / time.Duration(times) )
}

// Local Variables: //
// tab-width: 4 //
// End: //
