all: testcplusplus testTypescript

# C++ version original
testcplusplus:
	cd cpp ; make Makefilecpp
	
# Typescript
testTypescript:
	cd typescript ; make
