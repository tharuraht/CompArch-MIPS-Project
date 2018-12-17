# Example usage:
# If you have testbench/my_test.c, you can execute the following to create a MIPS binary
# make testbench/my_test.mips.bin

# For simulator
CC = g++
CPPFLAGS = -W -Wall

# For MIPS binaries. Turn on all warnings, enable all optimisations and link everything statically
MIPS_CC = mips-linux-gnu-gcc
MIPS_OBJCOPY = mips-linux-gnu-objcopy
MIPS_OBJDUMP = mips-linux-gnu-objdump
MIPS_CPPFLAGS = -W -Wall -O3 -fno-builtin -march=mips1 -mfp32
MIPS_LDFLAGS = -nostdlib -Wl,-melf32btsmip -march=mips1 -nostartfiles -mno-check-zero-division -Wl,--gpsize=0 -static -Wl,-Bstatic -Wl,--build-id=none


# Build simulator
bin/mips_simulator: src/simulator.cpp
	mkdir -p bin
	$(CC) $(CPPFLAGS) src/simulator.cpp -o bin/mips_simulator
	chmod u+x bin/mips_simulator

# Dummy for build simulator to conform to spec
simulator: bin/mips_simulator

# Dummy for build testbench to conform to spec. Could do nothing
testbench:
	mkdir -p bin
	mkdir -p test/test_binaries
	$(MAKE) -C mips-parser
	cp test/mips_testbench bin/
	chmod u+x bin/mips_testbench

clean:
	rm bin/mips_simulator
	rm bin/mips_testbench
	rm mips-parser/bin/parser
