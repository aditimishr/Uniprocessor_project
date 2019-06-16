# Uniprocessor Project

The implementation has several assumptions -
1) Each instruction and data are of 4 bytes
2) Instructions and data are stored hardcoded.
3) Two type of instructions where 10 is stored for read and 20 is stored for write.
4) And two types of data input are there - 
	a) if instruction, all the initial values are stored and data will be -1;
	b) if data type, store an integer value and instruction will be NULL;
5) Instructions are already present in L1_instruction_cache, L2_cache and Main_Memory
6) There is one file Input_File.txt which reads the instructions present in the L1_instruction cache


Important Note - For test case 8 and test case 9, I am reading two lines of instructions from Input_File.txt file
initial state - read 0
for test case 8 and 9 - read 0        (read 32 will only execute after read 0 according to the clear instructions)
			read 32		(And is seperated from "Read instructions from L1_instruction_cache")

Instructions to change the tese cases - 

1) Go to Mishra_Aditi_CSCI8510_Project\Aditi_ACA_Project\Row_of_a_main_memory and open the row.c file
2) All the test cases there are commented and can be executed one by one by uncommenting them.

Structure of the project - 

1) The project starts from main.c initializing the caches and running the processor.
2) Three caches are initialized - L1_Instruction_Cache, L1_Data_Cache and L2_Cache.
3) Structure of L1_Instruction or L1_Data_Cache -   			 // Refering to L1_Cache
	a) L1_Cache contains 64 sets.
	b) Each set has 4 cache lines.					// four way set associative
	c) Each cache line contains 8 data or instructions.		// Each line is 32 bytes and each data or instruction is 4 bytes
4) Structure of L2_Cache -
	a) L2_Cache contains 512 cache lines.
	b) Each cache line contains 8 data instructions.
5) Structure of Main_Memory -
	a) Main_Memory has 4096 rows equivalent to block of the cache.
	b) Each block of Main_Memory is also divided in 8 ints.


States used to acheive parallelism -

1) "P to L1C" -  CPU requests for L1 
2) "L1C to L1D : Hit"  - data found in L1D
3) "Victim to L1C"     - data not found in L1D but is present in Victim cache
4) "Write Buffer to LIC" - data not found in L1D but is present in Write Buffer
5) "L1C to L2C"  - data request to L2 controller
6) "L1D to L1C"  - data reply to L1 controller
7) "L1C to P"	 - while performing the read and returning data to cpu
8) "L1C to L1D and P" - when data was requested to L2 controller and while replying to L1 processor updating data in L1D
9) "L2C to Memory"	- when data not found in L2
10) "Memory to L2C"	- reply from memory to L2
11) "L2C to L1C"	- L2 reply to L1
12) "Memory to L2C : Block" - take 8 transfers to transfer a block from memory to L2
13) "Update L1C"	- write operation performed
14) "Wait" - in case of same addresses
15) "Done" - transaction done for the instruction


Test cases - 
1) test case 1   Single read
2) test case 2   Single write
3) test case 3   Single write followed by a read to the same address
4) test case 4      Back to back read to the same line
5) test case 5      Back to back write to the same line
6) test case 6    Back to back read to different line
7) test case 7      Back to back write to different lines
8) test case 8     Read from L1 victim cache
9) test case 9     Read from L1 Write Buffer

- The test cases and its corresponding traces are put in seperate folders



