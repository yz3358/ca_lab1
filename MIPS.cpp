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
            // implement the funciton by you.
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
                        rfout << Registers[j]<<endl;
                      }

                  }
                  else cout<<"Unable to open file";
                  rfout.close();

               }
	private:
            vector<bitset<32> >Registers;

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
          {       IMem.resize(MemSize);
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
                  else cout<<"Unable to open file";
                  fileout.close();

}


int main()
{
    bitset<32> pc=0;
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    while (1) //each loop body corresponds to one clock cycle.
	{






    // At the end of each cycle, fill in the corresponding data into "dumpResults" function to output files.
    // The first dumped pc value should be 0.
    dumpResults(pc, , , , , , );
    }
	      myRF.OutputRF(); // dump RF;
          myDataMem.OutputDataMem(); // dump data mem

          return 0;

}
