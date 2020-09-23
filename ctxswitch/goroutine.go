package main

import (
	"fmt"
	"time"
	"os"
	"strconv"
	"runtime"
)

//=======================================
// time context switch
//=======================================

var shake chan bool = make( chan bool )

func ContextSwitch(N int) {
	start := time.Now()
	for i := 1; i <= N; i += 1 {
		runtime.Gosched()
	}
	end := time.Now()
	fmt.Printf("%d\n", end.Sub(start) / time.Duration(N))
	shake <- true   // indicate completion
}

//=======================================
// benchmark driver
//=======================================

func main() {
	var times int = 10000000
	if len( os.Args ) > 2 { os.Exit( 1 ) }
	if len( os.Args ) == 2 { times, _ = strconv.Atoi(os.Args[1]) }
	go ContextSwitch( times )		// context switch
	<- shake
}

// Local Variables: //
// tab-width: 4 //
// End: //
