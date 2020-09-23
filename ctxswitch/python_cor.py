import sys
import time

times = 50000000
argc = len( sys.argv )
if argc > 2:
	sys.exit( 1 )
if argc == 2:
	times = int( sys.argv[1] )

def C():
	while True:
		yield

c = C()

start = time.time_ns()
for i in range( 10000000 ):
	next( c )  # resume
end = time.time_ns()
print( (end - start) / times )

# Local Variables: #
# tab-width: 4 #
# compile-command: "python3.7 python_cor.py" #
# End: #
