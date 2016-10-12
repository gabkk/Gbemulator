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
	 * Everything can be done with the Cpu object that has access to Memory and Registers.
	 *
	 * _instructions[OPCODE] = {						(for OPCODE from 0x00 to 0xff)
	 *    MNEMONIC_NAME,			(std::string)
	 *    SIZE,						(int)
	 *    NUMBER_OF_CLOCK CYCLES,	(int)
	 *    FUNCTION					(void (*)(Cpu *cpu)
	 * }
	 *
	 * There are 2 types of cycles. We use CLOCK CYCLES to be more precise.
	 * 1 machine cycle = 4 clock cycles
	 *
	 *                  GB CPU Speed   NOP Instruction
	 * Machine Cycles     1.05MHz         1 cycle
	 * Clock Cycles       4.19MHz         4 cycles
	 *
	 * Instruction cycles duration:
	 * <DMG> 0.954 μs (source oscillation: 4.1943 MHz)
	 * <CGB> 0.954 μs/0.477 μs, switchable (source oscillation: 8.3886 MHz)
	 * 
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
		3,
		12,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setB(mem->getByteAt(regs->getPC() + 1));
			regs->setC(mem->getByteAt(regs->getPC() + 2));
		}
	};

	_instructions[0x2] = {
		"LD (BC),A",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			mem->setByteAt(regs->getBC(), regs->getA());
		}
	};

	_instructions[0x3] = {
		"INC BC",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setBC(regs->getBC() + 1);			// increment BC
		}
	};

	_instructions[0x4] = {
		"INC B",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		b;

			b = regs->getB();
			regs->setFz(b + 1 == 0);			// set zero (Z) flag if b == 0 after INC
			regs->setFn(false);					// clear substract (N) flag
			regs->setFh(((b + 1) & 0x10) != 0);	// set half carry (H) flag if (b & 0x10) != 0 after INC
			regs->setFc(false);					// clear carry (C) flag
			regs->setB(b + 1);					// inc B
		}
	};

	_instructions[0x5] = {
		"DEC B",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		b;

			b = regs->getB();
			regs->setFz(b - 1 == 0);			// set zero (Z) flag if b == 0 after DEC
			regs->setFn(true);					// set substract (N) flag
			regs->setFh(((b - 1) & 0x10) != 0);	// set half carry flag (H) if (b & 0x10) != 0 after DEC
			regs->setB(regs->getB() - 1);		// dec B
		}
	};

	_instructions[0x6] = {
		"LD B,d8",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setB(mem->getByteAt(regs->getPC() + 1));
		}
	};

	_instructions[0x7] = {
		"RLCA",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		a;

			a = regs->getA();
			regs->setFz(false);
			regs->setFn(false);
			regs->setFh(false);
			regs->setFc((a >> 7) != 0);
			regs->setA((a << 1) | (a >> 7));
		}
	};

	_instructions[0x8] = {
		"LD (a16),SP",
		3,
		20,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			uint16_t			addr;

			addr = mem->getByteAt(regs->getPC()); // get higher byte of the 16 bits address
			addr = (addr << 8) | mem->getByteAt(regs->getPC() + 1); // shift it and get lower
			mem->setByteAt(addr, mem->getByteAt(regs->getSP()));
		}
	};

	_instructions[0x9] = {
		"ADD HL,BC",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setHL(regs->getHL() + regs->getBC()); // add BC to HL
			regs->setFn(false); // reset N flag
			//TODO
		}
	};

	_instructions[0xa] = {
		"LD A,(BC)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb] = {
		"DEC BC",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc] = {
		"INC C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd] = {
		"DEC C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe] = {
		"LD C,d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf] = {
		"RRCA",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x10] = {
		"STOP 0",
		2,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x11] = {
		"LD DE,d16",
		3,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x12] = {
		"LD (DE),A",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x13] = {
		"INC DE",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x14] = {
		"INC D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x15] = {
		"DEC D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x16] = {
		"LD, D,d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x17] = {
		"RLA",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x18] = {
		"JR r8",
		2,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x19] = {
		"ADD HL,DE",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1a] = {
		"LD A,(DE)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1b] = {
		"DEC DE",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1c] = {
		"INC E",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1d] = {
		"DEC E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1e] = {
		"LD E,d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x1f] = {
		"RRA",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x20] = {
		"JR NZ,r8",
		2,
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x21] = {
		"LD HL,d16",
		3,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x22] = {
		"LD (HL+),A",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x23] = {
		"INC HL",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x24] = {
		"INC H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x25] = {
		"DEC H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x26] = {
		"LD H,d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x27] = {
		"DAA",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x28] = {
		"JR Z,r8",
		2,
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x29] = {
		"ADD HL,HL",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2a] = {
		"LD A,(HL+)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2b] = {
		"DEC HL",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2c] = {
		"INC L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2d] = {
		"DEC L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2e] = {
		"LD L,d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x2f] = {
		"CPL",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x30] = {
		"JR NC,r8",
		2,
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x31] = {
		"LD SP,d16",
		3,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x32] = {
		"LD (HL-),A",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x33] = {
		"INC SP",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x34] = {
		"INC (HL)",
		1,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x35] = {
		"DEC (HL)",
		1,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x36] = {
		"LD (HL),d8",
		2,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x37] = {
		"SCF",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x38] = {
		"JR C,r8",
		2,
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x39] = {
		"ADD HL,SP",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3a] = {
		"LD A,(HL-)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3b] = {
		"DEC SP",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3c] = {
		"INC A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3d] = {
		"DEC A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3e] = {
		"LD A,d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x3f] = {
		"CCF",
		1,
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
			(void)cpu;
		}
	};

	_instructions[0x41] = {
		"LD B,C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x42] = {
		"LD B,D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x43] = {
		"LD B,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x44] = {
		"LD B,H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x45] = {
		"LD B,L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x46] = {
		"LD B,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x47] = {
		"LD B,A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x48] = {
		"LD C,B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x49] = {
		"LD C,C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x4a] = {
		"LD C,D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x4b] = {
		"LD C,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x4c] = {
		"LD C,H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x4d] = {
		"LD C,L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x4e] = {
		"LD C,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x4f] = {
		"LD C,A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x50] = {
		"LD D,B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x51] = {
		"LD D,C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x52] = {
		"LD D,D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x53] = {
		"LD D,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x54] = {
		"LD D,H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x55] = {
		"LD D,L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x56] = {
		"LD D,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x57] = {
		"LD D,A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x58] = {
		"LD E,B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x59] = {
		"LD E,C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x5a] = {
		"LD E,D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x5b] = {
		"LD E,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x5c] = {
		"LD E,H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x5d] = {
		"LD E,L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x5e] = {
		"LD E,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x5f] = {
		"LD E,A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x60] = {
		"LD H,B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x61] = {
		"LD H,C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x62] = {
		"LD H,D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x63] = {
		"LD H,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x64] = {
		"LD H,H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x65] = {
		"LD H,L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x66] = {
		"LD H,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x67] = {
		"LD H,A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x68] = {
		"LD L,B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x69] = {
		"LD L,C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x6a] = {
		"LD L,D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x6b] = {
		"LD L,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x6c] = {
		"LD L,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x6d] = {
		"LD L,L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x6e] = {
		"LD L,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x6f] = {
		"LD L,A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x70] = {
		"LD (HL),B",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x71] = {
		"LD (HL),C",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x72] = {
		"LD (HL),D",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x73] = {
		"LD (HL),E",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x74] = {
		"LD (HL),H",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x75] = {
		"LD (HL),L",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x76] = {
		"HALT",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x77] = {
		"LD (HL),A",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x78] = {
		"LD A,B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x79] = {
		"LD A,C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x7a] = {
		"LD A,D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x7b] = {
		"LD A,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x7c] = {
		"LD A,H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x7d] = {
		"LD A,L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x7e] = {
		"LD A,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x7f] = {
		"LD A,A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x80] = {
		"ADD A,B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x81] = {
		"ADD A,C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x82] = {
		"ADD A,D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x83] = {
		"ADD A,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x84] = {
		"ADD A,H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x85] = {
		"ADD A,L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x86] = {
		"ADD A,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x87] = {
		"ADD A,A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x88] = {
		"ADC A,B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x89] = {
		"ADC A,C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8a] = {
		"ADC A,D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8b] = {
		"ADC A,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8c] = {
		"ADC A,H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8d] = {
		"ADC A,L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8e] = {
		"ADC A,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x8f] = {
		"ADC A,A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x90] = {
		"SUB B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x91] = {
		"SUB C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x92] = {
		"SUB D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x93] = {
		"SUB E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x94] = {
		"SUB H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x95] = {
		"SUB L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x96] = {
		"SUB (HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x97] = {
		"SUB A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x98] = {
		"SBC A,B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x99] = {
		"SBC A,C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9a] = {
		"SBC A.D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9b] = {
		"SBC A,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9c] = {
		"SBC A,E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9d] = {
		"SBC A,H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9e] = {
		"SBC A,(HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0x9f] = {
		"SBC A,A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa0] = {
		"AND B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa1] = {
		"AND C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa2] = {
		"AND D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa3] = {
		"AND E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa4] = {
		"AND H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa5] = {
		"AND L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa6] = {
		"AND (HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa7] = {
		"AND A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa8] = {
		"XOR B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xa9] = {
		"XOR C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xaa] = {
		"XOR D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xab] = {
		"XOR E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xac] = {
		"XOR H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xad] = {
		"XOR L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xae] = {
		"XOR (HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xaf] = {
		"XOR A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb0] = {
		"OR B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb1] = {
		"OR C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb2] = {
		"OR D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb3] = {
		"OR E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb4] = {
		"OR H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb5] = {
		"OR L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb6] = {
		"OR (HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb7] = {
		"OR A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb8] = {
		"CP B",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xb9] = {
		"CP C",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xba] = {
		"CP D",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xbb] = {
		"CP E",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xbc] = {
		"CP H",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xbd] = {
		"CP L",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xbe] = {
		"CP (HL)",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xbf] = {
		"CP A",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc0] = {
		"RET NZ",
		1,
		8, // 20 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc1] = {
		"POP BC",
		1,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc2] = {
		"JP NZ,a16",
		1,
		12, // 16 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc3] = {
		"JP a16",
		3,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc4] = {
		"CALL NZ,a16",
		3,
		12, // 24 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc5] = {
		"PUSH BC",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc6] = {
		"ADD A,d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc7] = {
		"RST 00H",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc8] = {
		"RET Z",
		1,
		8, // 20 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xc9] = {
		"RET",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xca] = {
		"JP Z,a16",
		3,
		12, // 16 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xcb] = {
		"PREFIX CB",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xcc] = {
		"CALL Z,a16",
		3,
		12, // 24 if action taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xcd] = {
		"CALL a16",
		3,
		24,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xce] = {
		"ADC A,d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xcf] = {
		"RST 08H",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd0] = {
		"RET NC",
		1,
		8, // 20 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd1] = {
		"POP DE",
		1,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd2] = {
		"JP NC,a16",
		3,
		12, // 16 is jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd3] = {
		"OPCODE 0xd3 UNDEFINED",
		0,
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd4] = {
		"CALL NC,a16",
		3,
		12, // 24 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd5] = {
		"PUSH DE",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd6] = {
		"SUB d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd7] = {
		"RST 10H",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd8] = {
		"RET C",
		1,
		8, // 20 if action is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xd9] = {
		"RETI",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xda] = {
		"JP C,a16",
		3,
		12, // 16 if jump is taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xdb] = {
		"OPCODE 0xdb UNDEFINED",
		0,
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xdc] = {
		"CALL C,a16",
		3,
		12, // 24 if actio nis taken
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xdd] = {
		"OPCODE 0xdd UNDEFINED",
		0,
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xde] = {
		"SBC A,d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xdf] = {
		"RST 18H",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe0] = {
		"LDH (a8),A",
		2,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe1] = {
		"POP HL",
		1,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe2] = {
		"LD (C), A",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe3] = {
		"OPCODE 0xe3 UNDEFINED",
		0,
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe4] = {
		"OPCODE 0xe4 UNDEFINED",
		0,
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe5] = {
		"PUSH HL",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe6] = {
		"AND d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe7] = {
		"RST 20H",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe8] = {
		"ADD SP,r8",
		2,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xe9] = {
		"JP (HL)",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xea] = {
		"LD (a16),A",
		3,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xeb] = {
		"OPCODE 0xEB UNDEFINED",
		0,
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xec] = {
		"OPCODE 0xEC UNDEFINED",
		0,
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xed] = {
		"OPCODE 0xED UNDEFINED",
		0,
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xee] = {
		"XOR d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xef] = {
		"RST 28H",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf0] = {
		"LDH A,(a8)",
		2,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf1] = {
		"POP AF",
		1,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf2] = {
		"LD A,(C)",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf3] = {
		"DI",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf4] = {
		"OPCODE 0xF4 UNDEFINED",
		0,
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf5] = {
		"PUSH AF",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf6] = {
		"OR d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf7] = {
		"RST 30H",
		1,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf8] = {
		"LD HL,SP+r8",
		2,
		12,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xf9] = {
		"LD SP,HL",
		1,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xfa] = {
		"LD A,(a16)",
		3,
		16,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xfb] = {
		"EI",
		1,
		4,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xfc] = {
		"OPCODE 0xFC UNDEFINED",
		0,
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xfd] = {
		"OPCODE 0xFD UNDEFINED",
		0,
		0,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xfe] = {
		"CP d8",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
		}
	};

	_instructions[0xff] = {
		"RST 38H",
		1,
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

	instruction = &_instructions[opcode];
	instruction->exec(_cpu);
	_cpu->regs()->setPC(_cpu->regs->getPC() + instruction->length);
}
