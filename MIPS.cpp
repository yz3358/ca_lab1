//
// Created by ZhangYutong on 10/16/17.
//

#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.

/* Contains 32 32-bit registers defined as a private member.
 * Register $0 is always 0
 * TODO: implement the "ReadWrite()", that provides read and write access to the register file
*/
bitset<32>
convert_ulong_to_bitset(u_long) {
    bitset<32> result;
    return result;

}

class RF
{
public:
    bitset<32> ReadData1, ReadData2;
    RF()
    {
        Registers.resize(32);
        Registers[0] = bitset<32> (0);
    }

    void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
    {
        // implement the function by you.
        // Write always happen before read
        if (WrtEnable.all())
            Registers[(int) WrtReg.to_ulong()] = WrtData;

        ReadData1 = Registers[(int)RdReg1.to_ulong()];
        ReadData2 = Registers[(int)RdReg2.to_ulong()];

    }

    void OutputRF()
    {
        ofstream rfout;
        rfout.open("RFresult.txt",std::ios_base::app);
        if (rfout.is_open())
        {
            rfout<<"A state of RF:"<<endl;
            for (int j = 0; j<32; j++)
            {
                rfout << Registers[j] << endl;
            }

        }
        else cout<<"Unable to open file";
        rfout.close();

    }
private:
    vector<bitset<32> > Registers;

};
/* Implement the ALU
 * TODO: "ALUOperation()"
 * Performs the appropriate operation on two 32-bit operands based on ALUOP
*/
class ALU
{
public:
    bitset<32> ALUresult;
    bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
    {
        // implement the ALU operations by you.
        int aluop = (int) ALUOP.to_ulong();

        switch (aluop) {
            case ADDU:

                ALUresult = bitset<32> (oprand1.to_ulong() + oprand2.to_ulong()); // BE CAREFUL, might not work

            case SUBU:
                ALUresult = bitset<32> (oprand1.to_ulong() - oprand2.to_ulong()); // BE CAREFUL, might not work

            case AND:
                ALUresult = oprand1.operator&=(oprand2);

            case OR:
                ALUresult = oprand1.operator|=(oprand2);

            case NOR:
                ALUresult = oprand1.operator|=(oprand2);
                ALUresult = ALUresult.operator~();

            default:
                ALUresult = NULL;
        }
        return ALUresult;
    }
};

/* A byte addressable memory that contains instructions
 * The constructor, "INSMem()", initializes the contents of instruction memory from the "imem.txt"
 * TODO: implement "ReadMemory()"
 * Provides read access to your instruction memory
 * One access returns 4 bytes of data, that will form a whole 32-bit instruction
*/

class INSMem
{
public:
    bitset<32> Instruction;
    INSMem()
    {
        IMem.resize(MemSize);
        ifstream imem;
        string line;
        int i=0;
        imem.open("imem.txt");
        if (imem.is_open())
        {
            while (getline(imem,line))
            {
                IMem[i] = bitset<8>(line);
                i++;
            }

        }
        else cout<<"Unable to open file";
        imem.close();

    }

    bitset<32> ReadMemory (bitset<32> ReadAddress)
    {
        // implement by you. (Read the byte at the ReadAddress and the following three byte).
        int index = (int) ReadAddress.to_ulong();
        Instruction = bitset<32> (IMem[index].to_string()
                                  + IMem[index+1].to_string()
                                  + IMem[index+2].to_string()
                                  + IMem[index+3].to_string());
        // BE CAREFUL, may have different order; Should be "Big-Endian"
        return Instruction;
    }

private:
    vector<bitset<8> > IMem;

};

/* Similar to INSMem, but it should provide both read and write access.
*/

class DataMem
{
public:
    bitset<32> readdata;
    DataMem()
    {
        DMem.resize(MemSize);
        ifstream dmem;
        string line;
        int i=0;
        dmem.open("dmem.txt");
        if (dmem.is_open())
        {
            while (getline(dmem,line))
            {
                DMem[i] = bitset<8>(line);
                i++;
            }
        }
        else cout<<"Unable to open file";
        dmem.close();

    }
    bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
    {

        // implement by you.

        int index = (int) Address.to_ulong();
        if (readmem.all()) {
            readdata = bitset<32> (DMem[index].to_string()
                + DMem[index+1].to_string() + DMem[index+2].to_string() + DMem[index+3].to_string());
        } else if (writemem.all()) {
            string data_to_write = WriteData.to_string();
            // BE CAREFUL, may have different order
            DMem[index] = bitset<8> (data_to_write.substr(0,8));
            DMem[index+1] = bitset<8> (data_to_write.substr(8,8));
            DMem[index+2] = bitset<8> (data_to_write.substr(16,8));
            DMem[index+3] = bitset<8> (data_to_write.substr(24,8));
        }

        return readdata;
    }

    void OutputDataMem()
    {
        ofstream dmemout;
        dmemout.open("dmemresult.txt");
        if (dmemout.is_open())
        {
            for (int j = 0; j< 1000; j++)
            {
                dmemout << DMem[j]<<endl;
            }

        }
        else cout<<"Unable to open file";
        dmemout.close();

    }

private:
    vector<bitset<8> > DMem;

};

void dumpResults(bitset<32> pc, bitset<5> WrRFAdd, bitset<32> WrRFData, bitset<1> RFWrEn, bitset<32> WrMemAdd, bitset<32> WrMemData, bitset<1> WrMemEn)
{
    ofstream fileout;

    fileout.open("Results.txt",std::ios_base::app);
    if (fileout.is_open())
    {
        fileout <<pc<<' '<<WrRFAdd<<' '<<WrRFData<<' '<<RFWrEn<<' '<<WrMemAdd<<' '<<WrMemData<<' '<<WrMemEn << endl;
    }
    else cout<<"Unable to open file"<<endl;
    fileout.close();
}


int main()
{
    bitset<32> pc=0;
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    bitset<32> next_pc=0;
    bitset<32> instruction;
    string instruction_str = instruction.to_string();
    bitset<32> WrtData; // for RF
    bitset<32> WriteData; // for Datamem
    bitset<32> ReadData;
    bitset<32> Address;
    bitset<32> ALUresult;


    bitset<1> isEq;
    bitset<1> isBranch;
    bitset<1> isJ_Type;

    bitset<1> isLoad;
    bitset<1> isStore;
    bitset<1> isI_Type;
    bitset<3> ALUOP;
    bitset<1> WrtEnable; // re-set to 0 only when J-Type and I-Store ... (may be wrong)

    int counter_for_debuging = 0;

    while (1) //each loop body corresponds to one clock cycle.
    {
        // Stage 3: Data Mem load and Write back
        bitset<1> readmem = isLoad;
        bitset<1> writemem = isStore;
        ReadData = myDataMem.MemoryAccess(Address, WriteData, readmem, writemem);


        // Stage 2: Instruction Decode/RF read and Execution
        bitset<6> Opcode (instruction_str.substr(0,6));
        // cast the "opcode" as an int, easy to use
        int opcode = (int) Opcode.to_ulong();

        if (opcode == 0x23) isLoad.set();
        else isLoad.reset();

        if (opcode == 0x2B) isStore.set();
        else isStore.reset();

        if (opcode == 0x23 || opcode == 0x2B || opcode == 0x09 || opcode == 0x04)
            isI_Type.set();
        else isI_Type.reset();

        // ALUOp decoding
        if (opcode == 0x23 || opcode == 0x2B)
            ALUOP = bitset<3>(1);
        else if (opcode == 0x00)
            ALUOP = bitset<3>(instruction_str.substr(29,3));
        else
            ALUOP = bitset<3>(instruction_str.substr(3,3));

        if (opcode == 0x02 || opcode == 0x3F)
            isJ_Type.set();
        else isJ_Type.reset();

        if (opcode == 0x04) isBranch.set();
        else isBranch.reset();

        if (isStore.all() || isBranch.all() || isJ_Type.all())
            WrtEnable.reset();
        else WrtEnable.set();

        bitset<5> RdReg1(instruction_str.substr(6,5));
        bitset<5> RdReg2(instruction_str.substr(11,5));
        bitset<5> WrtReg(instruction_str.substr(16,5));
        if (isI_Type.all()) WrtReg = bitset<5>(instruction_str.substr(11,5));

        // In the 1st loop, the WrtData might not been initialized, but it will be initialized later at some point
        if (isLoad.all()) WrtData = ReadData;
        else WrtData = ALUresult;
        myRF.ReadWrite(RdReg1, RdReg2, WrtReg, WrtData, WrtEnable);
        if (myRF.ReadData1 == myRF.ReadData2) isEq.set();
        else isEq.reset();

        // sign extend
        bitset<32> signExtend;
        string imm = instruction_str.substr(16,16);
        if (imm[0]=='0')
            signExtend = bitset<32> ("0000000000000000" + imm);
        else
            signExtend = bitset<32> ("1111111111111111" + imm);

        bitset<32> oprand1 = myRF.ReadData1;
        bitset<32> oprand2 = myRF.ReadData2;
        if (isI_Type.all()) oprand2 = signExtend;
        ALUresult = myALU.ALUOperation(ALUOP, oprand1, oprand2);
        Address = ALUresult;



        // Stage 1: Instruction Fetch
        instruction = myInsMem.ReadMemory(pc);
        instruction_str = instruction.to_string();

        next_pc = bitset<32> (pc.to_ulong() + 4);
        if (isJ_Type.all())
            pc = bitset<32>(next_pc.to_string().substr(0,4) + instruction_str.substr(6,26) + "00");
        else if (isBranch.all())
            {if (isEq.all()) pc = bitset<32>(next_pc.to_ulong() + bitset<32>(signExtend.to_string().substr(2,30) + "00").to_ulong());}
        else pc = next_pc;

        // At the end of each cycle, fill in the corresponding data into "dumpResults" function to output files.
        // The first dumped pc value should be 0.
        // void dumpResults(bitset<32> pc, bitset<5> WrRFAdd, bitset<32> WrRFData, bitset<1> RFWrEn, bitset<32> WrMemAdd, bitset<32> WrMemData, bitset<1> WrMemEn)
        dumpResults(pc, WrtReg, WrtData, WrtEnable, Address, WriteData, writemem);

        if (opcode == 0x3F){
            printf("The program just halted.\n");
            cout << "Current pc is " << pc << endl;
            break;
        }  // halt
    }
    myRF.OutputRF(); // dump RF;
    myDataMem.OutputDataMem(); // dump data mem

    return 0;

}
