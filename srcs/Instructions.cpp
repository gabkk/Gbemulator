#include "../includes/Instructions.class.hpp"

/**
 * Instructions class constructor.
 * Depends on a Cpu instance, cannot be instanciated without it.
 * Initialize the Gameboy instruction set for the given cpu.
 *
 * @param cpu - The cpu we create the instruction set for
 */
Gbmu::Instructions::Instructions(Cpu *cpu) : _cpu(cpu) {
	/**
	 * We store the instructions in an array of structures containing a "lambda function" pointer.
	 * See (https://en.wikipedia.org/wiki/Anonymous_function#C.2B.2B_.28since_C.2B.2B11.29)
	 * and (http://stackoverflow.com/questions/30527865/static-array-of-lambda-functions-c)
	 * Everything can be done with the Cpu object that has access to Memory, Registers, PC/SP.
	 *
	 * From: http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
	 */
	_instructions[0x0] = { 
		"NOP",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1] = { 
		"LD BC,d16",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2] = { 
		"LD (BC),A",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3] = { 
		"INC BC",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x4] = { 
		"INC B",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x5] = { 
		"DEC B",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x6] = { 
		"LD B,d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x7] = { 
		"RLCA",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8] = { 
		"LD (a16),SP",
		20,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9] = { 
		"ADD HL,BC",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa] = { 
		"LD A,(BC)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb] = { 
		"DEC BC",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc] = { 
		"INC C",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd] = { 
		"DEC C",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe] = { 
		"LD C,d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf] = { 
		"RRCA",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x10] = { 
		"STOP 0",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x11] = { 
		"LD DE,d16",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x12] = { 
		"LD (DE),A",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x13] = { 
		"INC DE",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x14] = { 
		"INC D",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x15] = { 
		"DEC D",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x16] = { 
		"LD, D,d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x17] = { 
		"RLA",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x18] = { 
		"JR r8",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x19] = { 
		"ADD HL,DE",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1a] = { 
		"LD A,(DE)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1b] = { 
		"DEC DE",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1c] = { 
		"INC E",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1d] = { 
		"DEC E",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1e] = { 
		"LE E,d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1f] = { 
		"RRA",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x20] = { 
		"JR NZ,r8",
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x21] = { 
		"LD HL,d16",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x22] = { 
		"LD (HL+),A",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x23] = { 
		"INC HL",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x24] = { 
		"INC H",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x25] = { 
		"DEC H",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x26] = { 
		"LD H,d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x27] = { 
		"DAA",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x28] = { 
		"JR Z,r8",
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x29] = { 
		"ADD HL,HL",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2a] = { 
		"LD A,(HL+)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2b] = { 
		"DEC HL",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2c] = { 
		"INC L",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2d] = { 
		"DEC L",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2e] = { 
		"LD L,d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2f] = { 
		"CPL",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x30] = { 
		"JR NC,r8",
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x31] = { 
		"LD SP,d16",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x32] = { 
		"LD (HL-),A",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x33] = { 
		"INC SP",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x34] = { 
		"INC (HL)",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x35] = { 
		"DEC (HL)",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x36] = { 
		"LD (HL),d8",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x37] = { 
		"SCF",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x38] = { 
		"JR C,r8",
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x39] = { 
		"ADD HL,SP",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3a] = { 
		"LD A,(HL-)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3b] = { 
		"DEC SP",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3c] = { 
		"INC A",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3d] = { 
		"DEC A",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3e] = { 
		"LD A,d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3f] = { 
		"CCF",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x40] = { 
		"LD B,B",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB());
		}
	};

	_instructions[0x41] = { 
		"LD B,C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getC());
		}
	};

	_instructions[0x42] = { 
		"LD B,D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getD());
		}
	};

	_instructions[0x43] = { 
		"LD B,E",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getE());
		}
	};

	_instructions[0x44] = { 
		"LD B,H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getH());
		}
	};

	_instructions[0x45] = { 
		"LD B,L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getL());
		}
	};

	_instructions[0x46] = { 
		"LD B,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setB(mem->getByteAt(regs->getHL()));
		}
	};

	_instructions[0x47] = { 
		"LD B,A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getA());
		}
	};

	_instructions[0x48] = { 
		"LD C,B",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getB());
		}
	};

	_instructions[0x49] = { 
		"LD C,C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC());
		}
	};

	_instructions[0x4a] = { 
		"LD C,D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getD());
		}
	};

	_instructions[0x4b] = { 
		"LD C,E",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getE());
		}
	};

	_instructions[0x4c] = { 
		"LD C,H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getH());
		}
	};

	_instructions[0x4d] = { 
		"LD C,L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getL());
		}
	};

	_instructions[0x4e] = { 
		"LD C,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setC(mem->getByteAt(regs->getHL()));
		}
	};

	_instructions[0x4f] = { 
		"LD C,A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getA());
		}
	};

	_instructions[0x50] = { 
		"LD D,B",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getB());
		}
	};

	_instructions[0x51] = { 
		"LD D,C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getC());
		}
	};

	_instructions[0x52] = { 
		"LD D,D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD());
		}
	};

	_instructions[0x53] = { 
		"LD D,E",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getE());
		}
	};

	_instructions[0x54] = { 
		"LD D,H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getH());
		}
	};

	_instructions[0x55] = { 
		"LD D,L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getL());
		}
	};

	_instructions[0x56] = { 
		"LD D,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setD(mem->getByteAt(regs->getHL()));
		}
	};

	_instructions[0x57] = { 
		"LD D,A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getA());
		}
	};

	_instructions[0x58] = { 
		"LD E,B",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getB());
		}
	};

	_instructions[0x59] = { 
		"LD E,C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getC());
		}
	};

	_instructions[0x5a] = { 
		"LD E,D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getD());
		}
	};

	_instructions[0x5b] = { 
		"LD E,E",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE());
		}
	};

	_instructions[0x5c] = { 
		"LD E,H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getH());
		}
	};

	_instructions[0x5d] = { 
		"LD E,L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getL());
		}
	};

	_instructions[0x5e] = { 
		"LD E,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setE(mem->getByteAt(regs->getHL()));
		}
	};

	_instructions[0x5f] = { 
		"LD E,A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getA());
		}
	};

	_instructions[0x60] = { 
		"LD H,B",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getB());
		}
	};

	_instructions[0x61] = { 
		"LD H,C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getC());
		}
	};

	_instructions[0x62] = { 
		"LD H,D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getD());
		}
	};

	_instructions[0x63] = { 
		"LD H,E",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getE());
		}
	};

	_instructions[0x64] = { 
		"LD H,H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH());
		}
	};

	_instructions[0x65] = { 
		"LD H,L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getL());
		}
	};

	_instructions[0x66] = { 
		"LD H,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setH(mem->getByteAt(regs->getHL()));
		}
	};

	_instructions[0x67] = { 
		"LD H,A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getA());
		}
	};

	_instructions[0x68] = { 
		"LD L,B",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getB());
		}
	};

	_instructions[0x69] = { 
		"LD L,C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getC());
		}
	};

	_instructions[0x6a] = { 
		"LD L,D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getD());
		}
	};

	_instructions[0x6b] = { 
		"LD L,E",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getE());
		}
	};

	_instructions[0x6c] = { 
		"LD L,H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getH());
		}
	};

	_instructions[0x6d] = { 
		"LD L,L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL());
		}
	};

	_instructions[0x6e] = { 
		"LD L,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setL(mem->getByteAt(regs->getE()));
		}
	};

	_instructions[0x6f] = { 
		"LD L,A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getA());
		}
	};

	_instructions[0x70] = { 
		"LD (HL),B",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			mem->setByteAt(regs->getHL(), regs->getB());
		}
	};

	_instructions[0x71] = { 
		"LD (HL),C",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			mem->setByteAt(regs->getHL(), regs->getC());
		}
	};

	_instructions[0x72] = { 
		"LD (HL),D",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			mem->setByteAt(regs->getHL(), regs->getD());
		}
	};

	_instructions[0x73] = { 
		"LD (HL),E",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			mem->setByteAt(regs->getHL(), regs->getB());
		}
	};

	_instructions[0x74] = { 
		"LD (HL),H",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			mem->setByteAt(regs->getHL(), regs->getH());
		}
	};

	_instructions[0x75] = { 
		"LD (HL),L",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			mem->setByteAt(regs->getHL(), regs->getL());
		}
	};

	_instructions[0x76] = { 
		"HALT",
		1,
		4,
		[](Cpu *cpu) {
			if (this->onHalt() == true)
				this->setHALT = false;
			else
				this->setHALT = true;
		}
	};

	_instructions[0x77] = { 
		"LD (HL),A",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			mem->setByteAt(regs->getHL(), regs->getA());
		}
	};

	_instructions[0x78] = { 
		"LD A,B",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getB());
		}
	};

	_instructions[0x79] = { 
		"LD A,C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getC());
		}
	};

	_instructions[0x7a] = { 
		"LD A,D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getD());
		}
	};

	_instructions[0x7b] = { 
		"LD A,E",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getE());
		}
	};

	_instructions[0x7c] = { 
		"LD A,H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getH());
		}
	};

	_instructions[0x7d] = { 
		"LD A,L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getL());
		}
	};

	_instructions[0x7e] = { 
		"LD A,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setA(mem->getByteAt(regs->getHL()));
		}
	};

	_instructions[0x7f] = { 
		"LD A,A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA());
		}
	};

	_instructions[0x80] = { 
		"ADD A,B",
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		a;
			static uint8_t		b;

			a = regs->getA();
			b = regs->getB();
			regs->setFz(((a + b) & 0xff == 0));
			regs->setFn(false);
			regs->setFh(FLAG_H8_ADD(a, b));
			regs->setFh(FLAG_C8_ADD(a, b));
		}
	};

	_instructions[0x81] = { 
		"ADD A,C",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x82] = { 
		"ADD A,D",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x83] = { 
		"ADD A,E",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x84] = { 
		"ADD A,H",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x85] = { 
		"ADD A,L",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x86] = { 
		"ADD A,(HL)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x87] = { 
		"ADD A,A",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x88] = { 
		"ADC A,B",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x89] = { 
		"ADC A,C",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8a] = { 
		"ADC A,D",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8b] = { 
		"ADC A,E",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8c] = { 
		"ADC A,H",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8d] = { 
		"ADC A,L",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8e] = { 
		"ADC A,(HL)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8f] = { 
		"ADC A,A",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x90] = { 
		"SUB B",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x91] = { 
		"SUB C",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x92] = { 
		"SUB D",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x93] = { 
		"SUB E",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x94] = { 
		"SUB H",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x95] = { 
		"SUB L",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x96] = { 
		"SUB (HL)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x97] = { 
		"SUB A",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x98] = { 
		"SBC A,B",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x99] = { 
		"SBC A,C",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9a] = { 
		"SBC A.D",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9b] = { 
		"SBC A,E",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9c] = { 
		"SBC A,E",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9d] = { 
		"SBC A,H",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9e] = { 
		"SBC A,(HL)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9f] = { 
		"SBC A,A",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa0] = { 
		"AND B",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa1] = { 
		"AND C",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa2] = { 
		"AND D",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa3] = { 
		"AND E",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa4] = { 
		"AND H",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa5] = { 
		"AND L",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa6] = { 
		"AND (HL)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa7] = { 
		"AND A",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa8] = { 
		"XOR B",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa9] = { 
		"XOR C",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xaa] = { 
		"XOR D",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xab] = { 
		"XOR E",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xac] = { 
		"XOR H",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xad] = { 
		"XOR L",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xae] = { 
		"XOR (HL)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xaf] = { 
		"XOR A",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb0] = { 
		"OR B",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb1] = { 
		"OR C",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb2] = { 
		"OR D",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb3] = { 
		"OR E",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb4] = { 
		"OR H",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb5] = { 
		"OR L",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb6] = { 
		"OR (HL)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb7] = { 
		"OR A",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb8] = { 
		"CP B",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb9] = { 
		"CP C",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xba] = { 
		"CP D",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xbb] = { 
		"CP E",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xbc] = { 
		"CP H",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xbd] = { 
		"CP L",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xbe] = { 
		"CP (HL)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xbf] = { 
		"CP A",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc0] = { 
		"RET NZ",
		8, // 20 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc1] = { 
		"POP BC",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc2] = { 
		"JP NZ,a16",
		12, // 16 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc3] = { 
		"JP a16",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc4] = { 
		"CALL NZ,a16",
		12, // 24 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc5] = { 
		"PUSH BC",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc6] = { 
		"ADD A,d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc7] = { 
		"RST 00H",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc8] = { 
		"RET Z",
		8, // 20 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc9] = { 
		"RET",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xca] = { 
		"JP Z,a16",
		12, // 16 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xcb] = { 
		"PREFIX CB",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xcc] = { 
		"CALL Z,a16",
		12, // 24 if action taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xcd] = { 
		"CALL a16",
		24,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xce] = { 
		"ADC A,d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xcf] = { 
		"RST 08H",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd0] = { 
		"RET NC",
		8, // 20 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd1] = { 
		"POP DE",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd2] = { 
		"JP NC,a16",
		12, // 16 is jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd3] = { 
		"OPCODE 0xd3 UNDEFINED",
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd4] = { 
		"CALL NC,a16",
		12, // 24 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd5] = { 
		"PUSH DE",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd6] = { 
		"SUB d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd7] = { 
		"RST 10H",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd8] = { 
		"RET C",
		8, // 20 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd9] = { 
		"RETI",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xda] = { 
		"JP C,a16",
		12, // 16 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xdb] = { 
		"OPCODE 0xdb UNDEFINED",
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xdc] = { 
		"CALL C,a16",
		12, // 24 if actio nis taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xdd] = { 
		"OPCODE 0xdd UNDEFINED",
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xde] = { 
		"SBC A,d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xdf] = { 
		"RST 18H",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe0] = { 
		"LDH (a8),A",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe1] = { 
		"POP HL",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe2] = { 
		"LD (C), A",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe3] = { 
		"OPCODE 0xe3 UNDEFINED",
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe4] = { 
		"OPCODE 0xe4 UNDEFINED",
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe5] = { 
		"PUSH HL",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe6] = { 
		"AND d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe7] = { 
		"RST 20H",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe8] = { 
		"ADD SP,r8",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe9] = { 
		"JP (HL)",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xea] = { 
		"LD (a16),A",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xeb] = { 
		"OPCODE 0xEB UNDEFINED",
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xec] = { 
		"OPCODE 0xEC UNDEFINED",
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xed] = { 
		"OPCODE 0xED UNDEFINED",
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xee] = { 
		"XOR d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xef] = { 
		"RST 28H",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf0] = { 
		"LDH A,(a8)",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf1] = { 
		"POP AF",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf2] = { 
		"LD A,(C)",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf3] = { 
		"DI",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf4] = { 
		"OPCODE 0xF4 UNDEFINED",
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf5] = { 
		"PUSH AF",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf6] = { 
		"OR d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf7] = { 
		"RST 30H",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf8] = { 
		"LD HL,SP+r8",
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf9] = { 
		"LD SP,HL",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xfa] = { 
		"LD A,(a16)",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xfb] = { 
		"EI",
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xfc] = { 
		"OPCODE 0xFC UNDEFINED",
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xfd] = { 
		"OPCODE 0xFD UNDEFINED",
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xfe] = { 
		"CP d8",
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xff] = { 
		"RST 38H",
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};
}

/**
 * Instructions destructor.
 */
Gbmu::Instructions::~Instructions(void) {}

/**
 * Dispatcher function that calls the correct function based on the passed opcode
 *
 * @param opcode - The instruction we want to execute
 */
void Gbmu::Instructions::execute(uint8_t opcode) {
	static t_instruction	*instruction;
	static Registers		*regs = _cpu->regs();

	instruction = &_instructions[opcode];
	instruction->exec(_cpu);
	regs->setPC(regs->getPC() + instruction->size);
}