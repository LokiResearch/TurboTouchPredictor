all: testcplusplus testTypescript

# C++ version original
testcplusplus:
	cd cpp ; make -f Makefilecpp
	
# Typescript
testTypescript:
	cd typescript ; make
