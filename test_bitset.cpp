//
// Created by ZhangYutong on 10/16/17.
//

// C++ program to demonstrate various functionality of bitset

using namespace std;
#include <bitset>
#include <iostream>
#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>

#define M 32

void dumpResults(bitset<32> pc, bitset<5> WrRFAdd, bitset<32> WrRFData, bitset<1> RFWrEn, bitset<32> WrMemAdd, bitset<32> WrMemData, bitset<1> WrMemEn)
{
    ofstream fileout;

    fileout.open("Results.txt",std::ios_base::app);
    if (fileout.is_open())
    {
        fileout <<pc<<' '<<WrRFAdd<<' '<<WrRFData<<' '<<RFWrEn<<' '<<WrMemAdd<<' '<<WrMemData<<' '<<WrMemEn << endl;
    }
    else cout<<"Unable to open file";
    fileout.close();
}


int main()
{
    // default constructor initializes with all bits 0
    bitset<M> bset1;

    // bset2 is initialized with bits of 20
    bitset<M> bset2(20);

    // bset3 is initialized with bits of specified binary string
    bitset<M> bset3(string("1100"));

    // cout prints exact bits representation of bitset
    cout << bset1 << endl;  // 00000000000000000000000000000000
    cout << bset2 << endl;  // 00000000000000000000000000010100
    cout << bset3 << endl;  // 00000000000000000000000000001100
    cout << endl;

    // declaring set8 with capacity of 8 bits

    bitset<8> set8;    // 00000000

    // setting first bit (or 6th index)
    set8[1] = 1;    // 00000010
    set8[4] = set8[1];   //  00010010
    cout << set8 << endl;

    // count function returns number of set bits in bitset
    int numberof1 = set8.count();

    // size function returns total number of bits in bitset
    // so there difference will give us number of unset(0)
    // bits in bitset
    int numberof0 = set8.size() - numberof1;
    cout << set8 << " has " << numberof1 << " ones and "
         << numberof0 << " zeros\n";

    // test function return 1 if bit is set else returns 0
    cout << "bool representation of " << set8 << " : ";
    for (int i = 0; i < set8.size(); i++)
        cout << set8.test(i) << " ";

    cout << endl;

    // any function returns true, if atleast 1 bit
    // is set
    if (!set8.any())
        cout << "set8 has no bit set.\n";

    if (!bset1.any())
        cout << "bset1 has no bit set.\n";

    // none function returns true, if none of the bit
    // is set
    if (!bset1.none())
        cout << "bset1 has all bit set\n";

    // bset.set() sets all bits
    cout << set8.set() << endl;

    //  bset.set(pos, b) makes bset[pos] = b
    cout << set8.set(4, 0) << endl;

    // bset.set(pos) makes bset[pos] = 1  i.e. default
    // is 1
    cout << set8.set(4) << endl;

    // reset function makes all bits 0
    cout << set8.reset(2) << endl;
    cout << set8.reset() << endl;

    // flip function flips all bits i.e.  1 <-> 0
    // and  0 <-> 1
    cout << set8.flip(2) << endl;
    cout << set8.flip() << endl;

    // Converting decimal number to binary by using bitset
    int num = 100;
    cout  << "\nDecimal number: " << num
          << "  Binary equivalent: " << bitset<8>(num) << endl;

    printf("\n\n===============My own test below===============\n\n");

    // My test:
    set8.set();
    set8[7] = 0;
    set8[6] = 0;
    set8[5] = 0;
    if (set8.to_string() != "00011111")
        cout << "Fuck no." << endl; // this fuck no should not be printed

    if (set8.to_ulong() == 31)
        cout << "This should appear in the output!" << endl;

    cout << set8.to_ulong() << endl;

    bitset<32> set32_1, set32_2;
    u_long result_ulong;
    set32_1.set();
    set32_2[0] = 1;

//    result_ulong = set32_1.to_ulong() + set32_2.to_ulong();
//    result_ulong -= 0xFFFFFFFF;

    result_ulong = set32_2.to_ulong() - set32_1.to_ulong();
    bitset<32> result32(result_ulong); // initialize the bitset with unsigned long

    cout << "The result is: " << result_ulong << endl;
    cout << "The result is: " << result32 << endl;

    // concatenate
    bitset<8> set8_1 ("11000000");
    bitset<8> set8_2 ("11111111");
    result32 = bitset<32> (set8_1.to_string() + set8_2.to_string());
    cout << "The result is: " << result32 << endl;

    // compare 2 bitset directly
    set8_1.set();
    set8_2.set();
    if (set8_1 == bitset<8>("11111111") )
        cout << "Yep." << endl;



    // test dumpResults

    bitset<32> pc=0;

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
    bitset<5> WrtReg;
    bitset<1> writemem;
    bitset<1> isStore;
    bitset<1> isI_Type;
    bitset<3> ALUOP;
    bitset<1> WrtEnable; // re-set to 0 only when J-Type and I-Store ... (may be wrong)

    dumpResults(pc, WrtReg, WrtData, WrtEnable, Address, WriteData, writemem);
    return 0;
}