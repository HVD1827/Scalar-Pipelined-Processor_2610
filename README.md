## Scalar Pipelined Processor 
   
### Aim
To understand the working of a simple scalar pipelined processor by simulating the different components at the software level and identifying the stalls produced due to branch instructions and dependencies among different instructions.

### Description
* The Pipeline has 5 stages, namely, Instruction Fetch (IF), Instruction Decode (ID), Execute (EX), Memory (MEM) and Write Back(WB).
* Instruction Fetch: In this stage, instruction is fetched from Instruction Cache.
* Decode: In this stage, the source and destination registers and the operation are identified and data of source registers is read into two temporary registers A and B, also the dependencies are indentified if any.
* Exexute: Here the ALU performs the desired operation and the result is stored in a register named ALUOutputcurr. Another variable ALUOutputprev is maintained to allow forwarding.
* Memory: In case of Load and Store instructions, the data is loaded (into LMD) or stored in DCache(D$) respectively.
* Write Back Stage: The destination register is updated with the value of ALUOutput in case of non-memory instructions and LMD in case of a Load instruction.
* Data Hazards are handled by keeping a track of the destination registers whose values aren't updated/written back through the vector datahazard.
* Control Hazards are handled using the check variable which is set to true whenever there is a Branch or Jump instruction and accordingly the pipeline in stalled.
  
### How to run the code
 1. To compile, use the command:
           `g++ main.cpp`
 2. After Compiling, to run the code, use the command:
            `./a.exe` or `./a.out`
