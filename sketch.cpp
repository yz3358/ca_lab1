class RF {
	void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable);
}

class ALU {
	bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2);
}

class INSMem {
	// returns instruction
	bitset<32> ReadMemory (bitset<32> ReadAddress);
}

class DataMem {
	// returns readdata;
	bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
}


if (opcode == 0) {
            // R-Type
            WrtEnable.set();

            
            
            

            bitset<3> ALUOP(instruction_str.substr(29,3));
            bitset<32> oprand1 = myRF.ReadData1;
            bitset<32> oprand2 = myRF.ReadData2;
            WrtData = myALU.ALUOperation(ALUOP, oprand1, oprand2);
        } // Start of I-Type
        else if (opcode==0x09 || opcode==0x23 || opcode==0x2B) {
            // addiu, lw and sw have many similar behaviours
            WrtEnable.set();

            bitset<5> RdReg1(instruction_str.substr(6,5));
            bitset<5> RdReg2(instruction_str.substr(11,5));
            bitset<5> WrtReg(instruction_str.substr(11,5));
            myRF.ReadWrite(RdReg1, RdReg2, WrtReg, WrtData, WrtEnable);

            bitset<3> ALUOP(1);
            bitset<32> oprand1 = myRF.ReadData1;
            string imm = instruction_str.substr(16,16);
            bitset<32> oprand2;
            // sign extension
            if (imm[0]=='0')
                oprand2 = bitset<32> ("0000000000000000" + imm);
            else
                oprand2 = bitset<32> ("1111111111111111" + imm);

            if (opcode==0x09)
                WrtData = myALU.ALUOperation(ALUOP, oprand1, oprand2); // addiu : R[rt] <- R[rs] + {signexten + imm}
            else if (opcode==0x23)
                Address = myALU.ALUOperation(ALUOP, oprand1, oprand2); // lw: R[rt] <- Mem[{SignExtendimm} + R[rs]]
            else
                WriteData = myRF.ReadData2;  // sw: Mem[{SignExtendimm} + R[rs]] <- R[rt]

        }
        else if (Opcode.to_ulong()==0x04) {
            // beq: if{R[rs] == R[rt]} branch to PC + 4 + Target (“PC relative”)
            // Target is {SignExtendImm, 00}, the last 2 bits are always 0
            WrtEnable.reset(); // when branching, WrtEnable is 0

            bitset<5> RdReg1(instruction_str.substr(6,5));
            bitset<5> RdReg2(instruction_str.substr(11,5));
            bitset<5> WrtReg(instruction_str.substr(11,5));
            myRF.ReadWrite(RdReg1, RdReg2, WrtReg, WrtData, WrtEnable);

            if (myRF.ReadData1 == myRF.ReadData2)

        }

        else if () {
            // J-Type : WrtEnable.reset()
            WrtEnable.reset();


        } else{
            // WRONG INSTRUCTION
        }