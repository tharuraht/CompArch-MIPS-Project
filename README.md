# CompArch-MIPS-Project
Description
-----------
This is a improvement on one of my 2nd year projects, where we created a MIPS simulator, capable of executing many MIPS-1 big endian binary instructions as input. A accompanying (but independent) testbench was also created to generate binaries and test simulators.
Note that although the simulator is designed to comply with the MIPS ISA, there are some additional features that were required as part of the assessment specification.
Some of the requirements of the specification were as follows:
- Binaries are passed to simulator as a command line parameter, as a binary file containing MIPS-I big endian instructions
- Input to the simulator uses stdin, which is requested on reads of memory location
- Output of simulator uses stdout, requested on writes to a memory location
- On exit, the low 8 bits of $2 are returned
- If an exception occurs, the return code will be negative (codes specified below)

The simulator is written in C++

The accompanying testbench is written in bash, and will take as command line paramater an executeable mips simulator


Compile & Run
-------------
To compile the simulator, when in the root directory type 'make simulator', which will produce an executeable 'mips_simulator' in the bin folder
To run the simulator with a binary file binary.bin, type 'bin/mips_simulator binary.bin'

To compile the testbench, whien in the root directory type 'make testbench', which produces an exectuable 'mips_testbench'
To run the testbench with an executeable sim, type 'bin/mips_testbench sim'

Memory-Map
----------

The memory map of the simulated process is as follows:

```
Offset     |  Length     | Name       | R | W | X |
-----------|-------------|------------|---|---|---|--------------------------------------------------------------------
0x00000000 |        0x4  | ADDR_NULL  |   |   | Y | Jumping to this address means the Binary has finished execution.
0x00000004 |  0xFFFFFFC  | ....       |   |   |   |
0x10000000 |  0x1000000  | ADDR_INSTR | Y |   | Y | Executable memory. The Binary should be loaded here.
0x11000000 |  0xF000000  | ....       |   |   |   |
0x20000000 |  0x4000000  | ADDR_DATA  | Y | Y |   | Read-write data area. Should be zero-initialised.
0x24000000 |  0xC000000  | ....       |   |   |   |
0x30000000 |        0x4  | ADDR_GETC  | Y |   |   | Location of memory mapped input. Read-only.
0x30000004 |        0x4  | ADDR_PUTC  |   | Y |   | Location of memory mapped output. Write-only.
0x30000008 | 0xCFFFFFF8  | ....       |   |   |   |
-----------|-------------|------------|---|---|---|--------------------------------------------------------------------
```

Credits
-------
This project makes use of the MIPS parser created by olly-larkin and NGovani, which can be found at https://github.com/olly-larkin/mips-parser
This parser was used with the testbench, by generating the binaries from the MIPS assembly code written in test/test_sources
