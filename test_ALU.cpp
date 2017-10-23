//
// Created by jake on 10/23/17.
//

#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
typedef unsigned long u_long;

#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536


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

ALU myALU;
int main () {
    bitset<32> ALUresult;
    bitset<3> ALUOP;
    bitset<32> oprand1 = bitset<32> ("00000000000000000000000000000000");
    bitset<32> oprand2 = bitset<32> ("00000000000000000000000000001000");
    ALUresult = myALU.ALUOperation(ALUOP, oprand1, oprand2);
    cout << ALUresult.to_string() << endl;
    return 0;
}
