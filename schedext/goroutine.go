package main

import (
	"fmt"
	"time"
	"os"
	"strconv"
)

func main() {
	shake := make( chan bool )
	ch := make( chan int )

	acceptor := func(times int) {
		var v int
		v += 1 // need usage
		for i := 0; i < times; i += 1 {
			select {
				case v = <- ch :
			}
		}
		shake <- true   // indicate completion
	}

	var times int = 10000000
	if len( os.Args ) > 2 { os.Exit( 1 ) }
	if len( os.Args ) == 2 { times, _ = strconv.Atoi(os.Args[1]) }

	go acceptor( times )
	start := time.Now()
	for i := 0; i < times; i += 1 {
		ch <- 1
	}
	end := time.Now()
	fmt.Printf("%d\n", end.Sub(start) / time.Duration(times))
	<- shake
}

// Local Variables: //
// tab-width: 4 //
// compile-command: "go run goroutine.go" //
// End: //
