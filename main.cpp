/*Simulation of a scalar pipelined processor having 5 Stages: fetch, decode, execute, memory access, write back*/

#include <bits/stdc++.h>
using namespace std;
#define start_PC 0
#define int unsigned int

int32_t main()
{
    ifstream ifile;
    ifstream ifile1;
    ifstream ifile2;
    int x;
    int D$[256]; // stores the content of Data Cache
    int I$[256]; // stores the content of Instruction Cache
    int RF[16];  // Register File
    int cnt = 0;
    ifile.open("input/DCache.txt", ios ::in);
    while (cnt != 256 && ifile >> hex >> x)
    {
        D$[cnt] = x;
        cnt++;
    }

    cnt = 0;
    ifile1.open("input/ICache.txt", ios ::in);
    while (ifile1 >> hex >> x)
    {
        I$[cnt] = x;
        cnt++;
    }

    cnt = 0;
    ifile2.open("input/RF.txt", ios ::in);
    while (cnt != 16 && ifile2 >> hex >> x)
    {
        RF[cnt] = x;
        cnt++;
    }

    int PC = start_PC;
    int num_of_instructions = 1;
    int num_of_cycles = 0;
    // opcode format : opcode rd rs1 rs2
    int y = I$[PC + 1];
    int ifetch = (I$[PC] << 8);
    ifetch += y;
    int id = 1e9;
    int exec = 1e9;
    int mem = 1e9;
    int wb = 1e9;
    int a, bprev, bcurr;
    int ALUOutputprev, ALUOutputcurr;
    int LMD;
    bool cond = false;
    int reg_status[16] = {0};
    int stallnum = 0;
    // WHILE LOOP STARTS HERE and variables are declared as their name suggests//
    int rawstalls = 0;     // number of read after write stalls
    int controlstalls = 0; // number of control stalls
    int arith_ins = 0;
    int logical_ins = 0;
    int shift_ins = 0;
    int memory_ins = 0;
    int control_ins = 0;
    int halt_ins = 0;
    int loadimm_ins = 0;
    int opco = (ifetch >> 12);
    if (opco < 4)
        arith_ins++;
    else if (opco < 8)
        logical_ins++;
    else if (opco < 10)
        shift_ins++;
    else if (opco < 11)
        loadimm_ins++;
    else if (opco < 13)
        memory_ins++;
    else if (opco < 15)
        control_ins++;
    else
        halt_ins++;
    while (true)
    {
        num_of_cycles++;
        if (wb != 1e9)
        {
            int opc = (wb >> 12);
            int rdes = wb >> 8;
            rdes = (rdes & 0x0f);
            int imm = wb >> 4;
            imm = (imm & 0x0ff);
            if (opc <= 9)
            {
                RF[rdes] = ALUOutputprev;
                reg_status[rdes]--;
            }
            else if (opc == 10)
            {
                int imm1 = (wb & 0x0ff);
                RF[rdes] = imm1;
                reg_status[rdes]--;
            }
            else if (opc == 11)
            {
                RF[rdes] = LMD;
                reg_status[rdes]--;
            }
            if (opc == 15)
                break;
        }
        if (mem != 1e9)
        {
            ALUOutputprev = ALUOutputcurr;
            int opc = (mem >> 12);
            if (opc == 11)
                LMD = D$[ALUOutputprev];
            else if (opc == 12)
            {
                D$[ALUOutputprev] = bprev;
            }
        }

        if (exec != 1e9)
        {
            bprev = bcurr;
            int opc = (exec >> 12);
            if (opc == 0)
            {
                ALUOutputcurr = a + bprev;
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
            }
            else if (opc == 1)
            {
                ALUOutputcurr = a - bprev;
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
            }
            else if (opc == 2)
            {
                ALUOutputcurr = a * bprev;
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
            }
            else if (opc == 3)
            {
                ALUOutputcurr = a + 1;
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
            }
            else if (opc == 4)
            {
                ALUOutputcurr = (a & bprev);
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
            }
            else if (opc == 5)
            {
                ALUOutputcurr = (a | bprev);
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
            }
            else if (opc == 6)
            {
                ALUOutputcurr = (a ^ bprev);
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
            }
            else if (opc == 7)
            {
                ALUOutputcurr = (a ^ 0x0ff);
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
            }
            else if (opc == 8)
            {
                int imm = (exec & 0x0f);
                ALUOutputcurr = (a << imm);
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
            }
            else if (opc == 9)
            {
                int imm = (exec & 0x0f);
                ALUOutputcurr = (a >> imm);
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
            }
            else if (opc == 11 || opc == 12)
            {
                int imm = (exec & 0x0f);
                if (imm > 7)
                {
                    imm -= 16;
                }
                ALUOutputcurr = (a + imm);
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
            }
            else if (opc == 13)
            {
                int L1 = (exec & 0x0fff);
                L1 = (L1 >> 4);
                L1 = (L1 << 1);
                L1 = (L1 & 0x0ff);
                ALUOutputcurr = (PC + L1);
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
                PC = ALUOutputcurr;
                stallnum = 0;
            }
            else if (opc == 14)
            {
                cond = (a == 0);
                int L1 = (exec & 0x0ff);
                L1 = (L1 << 1);
                L1 = (L1 & 0x0ff);
                ALUOutputcurr = (PC + L1);
                ALUOutputcurr = (ALUOutputcurr & 0x0ff);
                if (cond)
                {
                    PC = ALUOutputcurr;
                }

                stallnum = 0;
            }
            else if (opc == 15)
            {
                ifetch = 1e9;
                stallnum = 100;
            }
        }

        if (id != 1e9)
        {

            int opc = (id >> 12);
            int rs2 = (id & 0x0f);
            int dummyid = (id >> 4);
            int rs1 = (dummyid & 0x0f);
            if (opc == 0 || opc == 1 || opc == 2 || opc == 4 || opc == 5 || opc == 6)
            {
                if (reg_status[rs1] == 0 && reg_status[rs2] == 0)
                {
                    a = RF[rs1];
                    bcurr = RF[rs2];
                    int rd = (id >> 8);
                    rd = (rd & 0x0f);
                    reg_status[rd]++;
                    //++;
                    wb = mem;
                    mem = exec;
                    exec = id;
                    id = ifetch;
                    if (stallnum == 0) // fr halt
                    {
                        PC += 2;
                        num_of_instructions++;
                        int x = I$[PC + 1];
                        ifetch = (I$[PC] << 8);
                        ifetch += x;
                        int l = (ifetch >> 12);
                        if (l == 15)
                        {
                            stallnum = 1000;
                        }
                        if (l == 13 || l == 14)
                            stallnum = 2;
                        int opco = (ifetch >> 12);
                        if (opco < 4)
                            arith_ins++;
                        else if (opco < 8)
                            logical_ins++;
                        else if (opco < 10)
                            shift_ins++;
                        else if (opco < 11)
                            loadimm_ins++;
                        else if (opco < 13)
                            memory_ins++;
                        else if (opco < 15)
                            control_ins++;
                        else
                            halt_ins++;
                    }
                    else
                    {
                        if (stallnum == 2)
                            controlstalls++;
                        ifetch = 1e9;
                        // stallnum--;
                    }
                    continue;
                }
                else
                {
                    rawstalls++;
                    wb = mem;
                    mem = exec;
                    exec = 1e9;
                    continue;
                }
            }

            else if (opc == 3)
            {
                int r = (id >> 8);
                r = (r & 0x0f);
                if (reg_status[r] == 0)
                {
                    a = RF[r];
                    int rd = (id >> 8);
                    rd = (rd & 0x0f);
                    reg_status[rd]++;
                    //++;
                    wb = mem;
                    mem = exec;
                    exec = id;
                    id = ifetch;
                    if (stallnum == 0)
                    {
                        PC += 2;
                        num_of_instructions++;
                        int x = I$[PC + 1];
                        ifetch = (I$[PC] << 8);
                        ifetch += x;
                        int l = (ifetch >> 12);
                        if (l == 15)
                        {
                            stallnum = 1000;
                        }
                        if (l == 13 || l == 14)
                            stallnum = 2;
                        int opco = (ifetch >> 12);
                        if (opco < 4)
                            arith_ins++;
                        else if (opco < 8)
                            logical_ins++;
                        else if (opco < 10)
                            shift_ins++;
                        else if (opco < 11)
                            loadimm_ins++;
                        else if (opco < 13)
                            memory_ins++;
                        else if (opco < 15)
                            control_ins++;
                        else
                            halt_ins++;
                    }
                    else
                    {
                        if (stallnum == 2)
                            controlstalls++;
                        ifetch = 1e9;
                        // stallnum--;
                    }
                    continue;
                }
                else
                {
                    rawstalls++;
                    wb = mem;
                    mem = exec;
                    exec = 1e9;
                    continue;
                }
            }
            else if (opc == 10)
            {
                int r = (id >> 8);
                r = (r & 0x0f);
                reg_status[r]++;
                //++;
            }
            else if (opc == 14)
            {
                int r = (id >> 8);
                r = (r & 0x0f);
                if (reg_status[r] == 0)
                {
                    a = RF[r];
                    //++;
                    wb = mem;
                    mem = exec;
                    exec = id;
                    id = ifetch;
                    if (stallnum == 0)
                    {
                        PC += 2;
                        num_of_instructions++;
                        int x = I$[PC + 1];
                        ifetch = (I$[PC] << 8);
                        ifetch += x;
                        int l = (ifetch >> 12);
                        if (l == 15)
                        {
                            stallnum = 1000;
                        }
                        if (l == 13 || l == 14)
                            stallnum = 2;
                        int opco = (ifetch >> 12);
                        if (opco < 4)
                            arith_ins++;
                        else if (opco < 8)
                            logical_ins++;
                        else if (opco < 10)
                            shift_ins++;
                        else if (opco < 11)
                            loadimm_ins++;
                        else if (opco < 13)
                            memory_ins++;
                        else if (opco < 15)
                            control_ins++;
                        else
                            halt_ins++;
                    }
                    else
                    {
                        if (stallnum == 2)
                            controlstalls++;
                        ifetch = 1e9;
                        // stallnum--;
                    }
                    continue;
                }
                else
                {
                    rawstalls++;
                    //++;
                    wb = mem;
                    mem = exec;
                    exec = 1e9;
                    continue;
                }
            }
            else if (opc == 7 || opc == 8 || opc == 9 || opc == 11)
            {
                if (reg_status[rs1] == 0)
                {
                    a = RF[rs1];
                    int rd = (id >> 8);
                    rd = (rd & 0x0f);
                    reg_status[rd]++;
                    //++;
                    wb = mem;
                    mem = exec;
                    exec = id;
                    id = ifetch;
                    if (stallnum == 0)
                    {
                        PC += 2;
                        num_of_instructions++;
                        int x = I$[PC + 1];
                        ifetch = (I$[PC] << 8);
                        ifetch += x;
                        int l = (ifetch >> 12);
                        if (l == 15)
                        {
                            stallnum = 1000;
                        }
                        if (l == 13 || l == 14)
                            stallnum = 2;
                        int opco = (ifetch >> 12);
                        if (opco < 4)
                            arith_ins++;
                        else if (opco < 8)
                            logical_ins++;
                        else if (opco < 10)
                            shift_ins++;
                        else if (opco < 11)
                            loadimm_ins++;
                        else if (opco < 13)
                            memory_ins++;
                        else if (opco < 15)
                            control_ins++;
                        else
                            halt_ins++;
                    }
                    else
                    {
                        if (stallnum == 2)
                            controlstalls++;
                        ifetch = 1e9;
                        // stallnum--;
                    }
                    continue;
                }
                else
                {
                    rawstalls++;
                    wb = mem;
                    mem = exec;
                    exec = 1e9;
                    continue;
                }
            }
            else if (opc == 12)
            {
                int r = (id >> 8);
                r = (r & 0x0f);
                if (reg_status[r] == 0 && reg_status[rs1] == 0)
                {
                    bcurr = RF[r];
                    int l = (id >> 4);
                    l = (l & 0x0f);
                    a = RF[l];
                    //++;
                    wb = mem;
                    mem = exec;
                    exec = id;
                    id = ifetch;
                    if (stallnum == 0)
                    {
                        PC += 2;
                        num_of_instructions++;
                        int x = I$[PC + 1];
                        ifetch = (I$[PC] << 8);
                        ifetch += x;
                        int l = (ifetch >> 12);
                        if (l == 15)
                        {
                            stallnum = 1000;
                        }
                        if (l == 13 || l == 14)
                            stallnum = 2;
                        int opco = (ifetch >> 12);
                        if (opco < 4)
                            arith_ins++;
                        else if (opco < 8)
                            logical_ins++;
                        else if (opco < 10)
                            shift_ins++;
                        else if (opco < 11)
                            loadimm_ins++;
                        else if (opco < 13)
                            memory_ins++;
                        else if (opco < 15)
                            control_ins++;
                        else
                            halt_ins++;
                    }
                    else
                    {
                        if (stallnum == 2)
                            controlstalls++;
                        ifetch = 1e9;
                        // stallnum--;
                    }
                    continue;
                }
                else
                {
                    rawstalls++;
                    wb = mem;
                    mem = exec;
                    exec = 1e9;
                    continue;
                }
            }
        }
        wb = mem;
        mem = exec;
        exec = id;
        id = ifetch;
        //++;
        if (stallnum == 0)
        {
            PC += 2;
            num_of_instructions++;
            int x = I$[PC + 1];
            ifetch = (I$[PC] << 8);
            ifetch += x;
            int l = (ifetch >> 12);
            int opc = (ifetch >> 12);
            if (opc < 4)
                arith_ins++;
            else if (opc < 8)
                logical_ins++;
            else if (opc < 10)
                shift_ins++;
            else if (opc < 11)
                loadimm_ins++;
            else if (opc < 13)
                memory_ins++;
            else if (opc < 15)
                control_ins++;
            else
                halt_ins++;
            if (l == 15)
            {
                stallnum = 1000;
            }
            if (l == 13 || l == 14)
                stallnum = 2;
        }
        else
        {
            if (stallnum == 2)
                controlstalls++;
            ifetch = 1e9;
        }
    }
    ofstream ofile;
    ofstream ofile1;
    ofile.open("output/DCache.txt", ios ::out);
    for (int i = 0; i < 256; i++)
    {
        if (D$[i] < 16)
            ofile << 0;
        ofile << hex << D$[i] << endl;
    }
    ofile1.open("output/Output.txt", ios ::out);
    ofile1 << "Total number of instructions executed        : " << num_of_instructions << endl;
    ofile1 << "Number of instructions in each class\n";
    ofile1 << "Arithmetic instructions                      : " << arith_ins << endl;
    ofile1 << "Logical instructions                         : " << logical_ins << endl;
    ofile1 << "Shift instructions                           : " << shift_ins << endl;
    ofile1 << "Memory instructions                          : " << memory_ins << endl;
    ofile1 << "Load immediate instructions                  : " << loadimm_ins << endl;
    ofile1 << "Control instructions                         : " << control_ins << endl;
    ofile1 << "Halt instructions                            : " << halt_ins << endl;
    ofile1 << "Cycles Per Instruction                       : " << ((float)num_of_cycles) / ((float)num_of_instructions) << endl;
    ofile1 << "Total number of stalls                       : " << num_of_cycles - num_of_instructions - 4 << endl;
    ofile1 << "Data stalls (RAW)                            : " << rawstalls << endl;
    ofile1 << "Control stalls                               : " << controlstalls << endl;
}