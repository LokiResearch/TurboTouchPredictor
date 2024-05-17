all: testcplusplus testTypescript testPython

# C++ version original
testcplusplus:
	cd cpp ; make -f Makefilecpp
	
# Typescript
testTypescript:
	cd typescript ; make

testPython:
	cd python ; make
