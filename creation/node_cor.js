var times = 50000000
var argc = process.argv.length // node, path
if ( argc > 3 ) process.exit( 1 )
if ( argc == 3 ) times = Number( process.argv[2] )

function * coroutine() { yield }

for ( var i = 0; i < times; i += 1 ) { // warm jit
	cor = coroutine()
}

var hrstart = process.hrtime()
for ( var i = 0; i < times; i += 1 ) {
	cor = coroutine()
}
hrend = process.hrtime( hrstart )
var dur = (1000000000 * hrend[0] + hrend[1]) / times
console.log( dur )

// Local Variables: //
// tab-width: 4 //
// compile-command: "nodejs node_cor.js" //
// End: //
