//
// Created by ZhangYutong on 10/18/17.
//

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
        else cout << "Unable to open file" << endl;
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

int main()
{
    INSMem myINSmem;
    bitset<32> address(0);
    bitset<32> instruction = myINSmem.ReadMemory(address);

    cout << instruction << endl;

    return 0;

}
