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
int IfZero = 0;// beq is true, those are equal

// Fetch any bits you like
bitset<32> Fetch_bits(bitset<32> bits, int first_num, int second_num){
	int Total_Num = first_num - second_num + 1;
	bitset<32>fetch_bits;
	string Str_Num = "";
	for (int i = 0; i < Total_Num; i++){
		Str_Num.append("1");
	}
	bitset<32>And_Bits(Str_Num);
	And_Bits = And_Bits << second_num;
	fetch_bits = bits & And_Bits;
	fetch_bits = fetch_bits >> second_num;
	return fetch_bits;
}


class Decoder{
public:
	bitset<3> aluop;
	bitset<2> Next_PC;
	bitset<1> wrt_reg, wrt_mem, rd_mem, reg2_imm, alu_mem, rt_rd;

	Decoder(){
		aluop = 0;
		Next_PC = 0;
		wrt_reg = 0;
		wrt_mem = 0;
		rd_mem = 0;
		reg2_imm = 0;
		alu_mem = 0;
		rt_rd = 0;
	}

	void decoder(bitset<32> instruction){
		addu_ins = 0, subu_ins = 0, addiu_ins = 0, and_ins = 0, or_ins = 0, nor_ins = 0, beq_ins = 0, lw_ins = 0, sw_ins = 0, j_ins = 0;
		if (Fetch_bits(instruction, 31, 26) == 0 && Fetch_bits(instruction, 5, 0) == 33) addu_ins = 1;
		else if (Fetch_bits(instruction, 31, 26) == 0 && Fetch_bits(instruction, 5, 0) == 35) subu_ins = 1;
		else if (Fetch_bits(instruction, 31, 26) == 9) addiu_ins = 1;
		else if (Fetch_bits(instruction, 31, 26) == 0 && Fetch_bits(instruction, 5, 0) == 36) and_ins = 1;
		else if (Fetch_bits(instruction, 31, 26) == 0 && Fetch_bits(instruction, 5, 0) == 37) or_ins = 1;
		else if (Fetch_bits(instruction, 31, 26) == 0 && Fetch_bits(instruction, 5, 0) == 39) nor_ins = 1;
		else if (Fetch_bits(instruction, 31, 26) == 4) beq_ins = 1;
		else if (Fetch_bits(instruction, 31, 26) == 35) lw_ins = 1;
		else if (Fetch_bits(instruction, 31, 26) == 43) sw_ins = 1;
		else if (Fetch_bits(instruction, 31, 26) == 2) j_ins = 1;
		else;
		//cout << instruction<< endl;
		if (addu_ins | addiu_ins | lw_ins | sw_ins)aluop = ADDU;
		else if (beq_ins | subu_ins)aluop = SUBU;
		else if (and_ins)aluop = AND;
		else if (or_ins)aluop = OR;
		else if (nor_ins)aluop = NOR;
		else;

		if (j_ins)Next_PC = 2;
		else Next_PC = 1;

		wrt_reg = (addu_ins | addiu_ins | lw_ins | subu_ins | and_ins | or_ins | nor_ins);
		wrt_mem = (sw_ins);
		rd_mem = (lw_ins);
		reg2_imm = (lw_ins | sw_ins | addiu_ins);
		alu_mem = (lw_ins);
		rt_rd = (addu_ins | subu_ins | and_ins | or_ins | nor_ins);

	}
	void IfBranch(int Zero){
		if (beq_ins && Zero == 0) Next_PC = 3;
	}
private:
	int addu_ins, subu_ins, addiu_ins, and_ins, or_ins, nor_ins, beq_ins, lw_ins, sw_ins, j_ins;
};
class RF{
public:
	bitset<32> ReadData1, ReadData2;
	RF(){       //初始化
		Registers.resize(32);
		Registers[0] = bitset<32>(0);
	}

	void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable){
		// implement the funciton by you.
		ReadData1 = Registers[RdReg1.to_ulong()];
		ReadData2 = Registers[RdReg2.to_ulong()];
		if (WrtEnable == bitset<1>(1)){
			Registers[WrtReg.to_ulong()] = WrtData;
		}
	}

	void OutputRF(){
		ofstream rfout;
		rfout.open("/home/jake/Desktop/ca_lab1/RFresult1.txt", std::ios_base::app);
		if (rfout.is_open()){
			rfout << "A state of RF:" << endl;
			for (int j = 0; j<32; j++){
				rfout << Registers[j] << endl;
			}
		}
		else cout << "Unable to open file";
		rfout.close();
	}
private:
	vector<bitset<32> >Registers;

};

class ALU{
public:
	bitset<32> ALUresult;
	bitset<32> ALUOperation(bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2){
		// implement the ALU operations by you. 
		if (ALUOP == ADDU){
			ALUresult = bitset<32>(oprand1.to_ulong() + oprand2.to_ulong());
		}
		else if (ALUOP == SUBU){
			ALUresult = bitset<32>(oprand1.to_ulong() - oprand2.to_ulong());
		}
		else if (ALUOP == AND){
			ALUresult = oprand1 & oprand2;
		}
		else if (ALUOP == OR){
			ALUresult = oprand1 | oprand2;
		}
		else if (ALUOP == NOR){
			ALUresult = ~(oprand1 | oprand2);
		}
		else ALUresult = 0;
		return ALUresult;
	}
};

class INSMem{
public:
	bitset<32> Instruction;
	INSMem(){
		IMem.resize(MemSize);
		ifstream imem;
		string line;
		int i = 0;
		imem.open("/home/jake/Desktop/ca_lab1/imem.txt");
		if (imem.is_open()){
			while (getline(imem, line)){
				IMem[i] = bitset<8>(line);
				i++;
			}
		}
		else cout << "Unable to open file";
		imem.close();
	}

	bitset<32> ReadMemory(bitset<32> ReadAddress){
		// implement by you. (Read the byte at the ReadAddress and the following three byte).
		bitset<32>Byte1, Byte2, Byte3, Byte4;
		Byte1 = bitset<32>(IMem[ReadAddress.to_ulong()].to_ulong());
		Byte2 = bitset<32>(IMem[ReadAddress.to_ulong() + 1].to_ulong());
		Byte3 = bitset<32>(IMem[ReadAddress.to_ulong() + 2].to_ulong());
		Byte4 = bitset<32>(IMem[ReadAddress.to_ulong() + 3].to_ulong());
		//cout << Byte1 << " " << Byte2 << endl;
		Instruction = Byte1<<24 | Byte2 << 16 | Byte3 << 8 | Byte4;
		return Instruction;
	}
private:
	vector<bitset<8> > IMem;
};

class DataMem{
public:
	bitset<32> readdata;
	DataMem(){
		DMem.resize(MemSize);
		ifstream dmem;
		string line;
		int i = 0;
		dmem.open("/home/jake/Desktop/ca_lab1/dmem.txt");
		if (dmem.is_open()){
			while (getline(dmem, line)){
				DMem[i] = bitset<8>(line);
				i++;
			}
		}
		else cout << "Unable to open file";
		dmem.close();

	}
	void MemoryAccess(bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem){
		// implement by you.
		if (readmem == 1){
			bitset<32>Byte1, Byte2, Byte3, Byte4;
			Byte1 = bitset<32>(DMem[Address.to_ulong()].to_ulong());
			Byte2 = bitset<32>(DMem[Address.to_ulong() + 1].to_ulong());
			Byte3 = bitset<32>(DMem[Address.to_ulong() + 2].to_ulong());
			Byte4 = bitset<32>(DMem[Address.to_ulong() + 3].to_ulong());
			readdata = Byte1<<24 | Byte2 << 16 | Byte3 << 8 | Byte4;
		}
		else;
		if (writemem == 1){
			bitset<8>Byte1, Byte2, Byte3, Byte4;
			Byte1 = bitset<8>(Fetch_bits(WriteData, 7, 0).to_ulong());
			Byte2 = bitset<8>(Fetch_bits(WriteData, 15, 8).to_ulong());
			Byte3 = bitset<8>(Fetch_bits(WriteData, 23, 16).to_ulong());
			Byte4 = bitset<8>(Fetch_bits(WriteData, 31, 24).to_ulong());
			DMem[Address.to_ulong()] = Byte4;
			DMem[Address.to_ulong() + 1] = Byte3;
			DMem[Address.to_ulong() + 2] = Byte2;
			DMem[Address.to_ulong() + 3] = Byte1;
		}
		else;
	}
	void OutputDataMem(){
		ofstream dmemout;
		dmemout.open("/home/jake/Desktop/ca_lab1/dmemresult1.txt");
		if (dmemout.is_open()){
			for (int j = 0; j< 1000; j++){
				dmemout << DMem[j] << endl;
			}
		}
		else cout << "Unable to open file";
		dmemout.close();

	}

private:
	vector<bitset<8> > DMem;

};

void dumpResults(bitset<32> pc, bitset<5> WrRFAdd, bitset<32> WrRFData, bitset<1> RFWrEn, bitset<32> WrMemAdd, bitset<32> WrMemData, bitset<1> WrMemEn)
{
    ofstream fileout;

    fileout.open("/home/jake/Desktop/ca_lab1/Results.txt",std::ios_base::app);
    if (fileout.is_open())
    {
        fileout <<pc<<' '<<WrRFAdd<<' '<<WrRFData<<' '<<RFWrEn<<' '<<WrMemAdd<<' '<<WrMemData<<' '<<WrMemEn << endl;
    }
    else cout<<"Unable to open file"<<endl;
    fileout.close();
}

int main()
{
	Decoder myDecoder;
	RF myRF;
	ALU myALU;
	INSMem myInsMem;
	DataMem myDataMem;

	bitset<32>All_One(4294967295);
	bitset<32>Npc(0);
    bitset<32>pc(0);
	bitset<5>rs, rt, rd;
	bitset<32>imm;
	bitset<5>write_reg_addr;
	bitset<32>write_reg_data;
	bitset<32>PC1, PC2, PC3;

	while (1)
	{
		// Fetch
        pc = Npc;
		myInsMem.ReadMemory(pc);
		// If current insturciton is "11111111111111111111111111111111", then break;		
		if (myInsMem.Instruction == All_One) break;
		else{
			// decode(Read RF)
			myDecoder.decoder(myInsMem.Instruction);
			rs = bitset<5>(Fetch_bits(myInsMem.Instruction, 25, 21).to_ulong());
			rt = bitset<5>(Fetch_bits(myInsMem.Instruction, 20, 16).to_ulong());
			rd = bitset<5>(Fetch_bits(myInsMem.Instruction, 15, 11).to_ulong());
			imm = bitset<32>(Fetch_bits(myInsMem.Instruction, 15, 0).to_ulong());
			myRF.ReadWrite(rs, rt, bitset<5>(0), bitset<32>(0), 0);
			//cout << myRF.ReadData1 << "  " << myRF.ReadData2 << endl;
			// Execute
			if (myDecoder.reg2_imm == 0){
				myALU.ALUOperation(myDecoder.aluop, myRF.ReadData1, myRF.ReadData2);
			}
			else myALU.ALUOperation(myDecoder.aluop, myRF.ReadData1, imm);

			// Read/Write Mem
			myDataMem.MemoryAccess(myALU.ALUresult, myRF.ReadData2, myDecoder.rd_mem, myDecoder.wrt_mem);
            if (myDecoder.wrt_mem.all()) {
                cout << "Write to data with address " << myALU.ALUresult.to_ulong() << endl;
                cout << "The data is " << myRF.ReadData2.to_ulong() << endl;
            }
			//cout << myDecoder.rd_mem<<"  "<<myDataMem.readdata<<endl;

			// Write back to RF
			if (myDecoder.rt_rd == 0)write_reg_addr = rt;
			else write_reg_addr = rd;
			if (myDecoder.alu_mem == 0)write_reg_data = myALU.ALUresult;
			else write_reg_data = myDataMem.readdata;
			myRF.ReadWrite(rs, rt, write_reg_addr, write_reg_data, myDecoder.wrt_reg);

			//Next PC
			PC1 = Npc.to_ulong() + 4;
			PC2 = (Fetch_bits(PC1, 31, 28) << 27) | (Fetch_bits(myInsMem.Instruction, 25, 0) << 2);
			if (Fetch_bits(imm, 15, 15) == 1) PC3 = PC1.to_ulong() - ((imm << 17) >> 15).to_ulong();
			else PC3 = PC1.to_ulong() + (imm << 2).to_ulong();
			myDecoder.IfBranch(myALU.ALUresult.to_ulong());
			if (myDecoder.Next_PC == 1)Npc = PC1;
			else if (myDecoder.Next_PC == 2){
                Npc = PC2;
            }
			else Npc = PC3;
			//cout << Npc<<endl;
            cout << "Current pc is " << pc.to_ulong() << endl;
            cout << "Current instruction is " << myInsMem.Instruction.to_string() << endl;
		}
		myRF.OutputRF(); // dump RF;
	}
	myDataMem.OutputDataMem(); // dump data mem
	return 0;
}

