#include "../includes/Instructions.class.hpp"

/**
 * Instructions class constructor.
 * Depends on a Cpu instance, cannot be instanciated without it.
 * regs->set( regs->get()  & ~(1 << z));
 *
 * @param cpu - The cpu we create the instruction set for
 *
 */

/*
* Subfunction for repetitive work
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
			regs->setFz(((b + 1) & 0xff) == 0);		// set zero (Z) flag if b == 0 after INC
			regs->setFn(false);				// clear substract (N) flag
			regs->setFh(FLAG_H8_ADD(b, 1));		// set half carry (H) flag if carry from bit 3
			regs->setB(b + 1);				// inc B
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
			regs->setFz(((b - 1) & 0xff) == 0);		// set zero (Z) flag if b == 0 after DEC
			regs->setFn(true);				// set substract (N) flag
			regs->setFh(FLAG_H8_SUB(b, -1));	// set half carry flag (H) if borrow from bit 4
			regs->setB(regs->getB() - 1);	// dec B
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
			regs->setFc(a >> 7);
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

			addr = mem->getByteAt(regs->getPC() + 1);				// get higher byte of the 16 bits address
			addr = (addr << 8) | mem->getByteAt(regs->getPC() + 2);	// shift it and get lower
			mem->setByteAt(addr, mem->getByteAt(regs->getSP()));	// write SP contents at addr
		}
	};

	_instructions[0x9] = {
		"ADD HL,BC",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint16_t		hl, bc;

			hl = regs->getHL();
			bc = regs->getBC();
			regs->setFn(false);					// reset N flag
			regs->setFh(FLAG_H16_ADD(hl, bc));		// set H if carry from bit 11
			regs->setFc(FLAG_C16_ADD(hl, bc));		// set C if carry from bit 15
			regs->setHL(hl + bc);				// add BC to HL
		}
	};

	_instructions[0xa] = {
		"LD A,(BC)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setA(mem->getByteAt(regs->getBC()));
		}
	};

	_instructions[0xb] = {
		"DEC BC",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setBC(regs->getBC() - 1);
		}
	};

	_instructions[0xc] = {
		"INC C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		c;

			c = regs->getC();
			regs->setFz(((c + 1) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_ADD(c, 1));
			regs->setC(regs->getC() + 1);
		}
	};

	_instructions[0xd] = {
		"DEC C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		c;

			c = regs->getC();
			regs->setFz(((c - 1) & 0xff) == 0);
			regs->setFn(true);
			regs->setFh(FLAG_H8_SUB(c, -1));
			regs->setC(regs->getC() - 1);
		}
	};

	_instructions[0xe] = {
		"LD C,d8",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setC(mem->getByteAt(regs->getPC() + 1));
		}
	};

	_instructions[0xf] = {
		"RRCA",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		a;

			a = regs->getA();
			regs->setFz(false);
			regs->setFn(false);
			regs->setFh(false);
			regs->setFc(a & 1);
			regs->setA((a >> 1) | (a << 7));
		}
	};

	_instructions[0x10] = {
		"STOP 0",
		2,
		4,
		[](Cpu *cpu) {
			(void)cpu;
			/**
			 * TODO: implement switching to STOP mode (stop processor & screen until button press)
			 * see: 
			 *   - http://www.chrisantonellis.com/files/gameboy/gb-programming-manual.pdf (p.22)
			 *   - http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf (p.19)
			 */
		}
	};

	_instructions[0x11] = {
		"LD DE,d16",
		3,
		12,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setD(mem->getByteAt(regs->getPC() + 1));
			regs->setE(mem->getByteAt(regs->getPC() + 2));
		}
	};

	_instructions[0x12] = {
		"LD (DE),A",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			mem->setByteAt(regs->getDE(), regs->getA());
		}
	};

	_instructions[0x13] = {
		"INC DE",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setDE(regs->getDE() + 1);
		}
	};

	_instructions[0x14] = {
		"INC D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		d;

			d = regs->getD();
			regs->setFz(((d + 1) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_ADD(d, 1));
			regs->setD(d + 1);
		}
	};

	_instructions[0x15] = {
		"DEC D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		d;

			d = regs->getD();
			regs->setFz(((d - 1) & 0xff) == 0);
			regs->setFn(true);
			regs->setFh(FLAG_H8_SUB(d, -1));
			regs->setD(d - 1);
		}
	};

	_instructions[0x16] = {
		"LD D,d8",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setD(mem->getByteAt(regs->getPC() + 1));
		}
	};

	_instructions[0x17] = {
		"RLA",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			uint8_t				a;

			a = regs->getA();
			regs->setA(a << 1 | regs->getFc());
			regs->setFz(false);
			regs->setFn(false);
			regs->setFh(false);
			regs->setFc(a >> 7);
		}
	};

	_instructions[0x18] = {
		"JR r8", // r8 = offset - 2 we go to (because we do pc += instruction.size, here 2)
		2,
		12,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setPC(regs->getPC() + static_cast<int8_t>(mem->getByteAt(regs->getPC() + 1)));
		}
	};

	_instructions[0x19] = {
		"ADD HL,DE",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			uint16_t			hl, de;

			hl = regs->getHL();
			de = regs->getDE();
			regs->setFn(false);
			regs->setFh(FLAG_H16_ADD(hl, de));
			regs->setFc(FLAG_C16_ADD(hl, de));
			regs->setHL(hl + de);
		}
	};

	_instructions[0x1a] = {
		"LD A,(DE)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setA(mem->getByteAt(regs->getDE()));
		}
	};

	_instructions[0x1b] = {
		"DEC DE",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setDE(regs->getDE() - 1);
		}
	};

	_instructions[0x1c] = {
		"INC E",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			uint8_t				e;

			e = regs->getE();
			regs->setFz(((e + 1) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_ADD(e, 1));
			regs->setE(regs->getE() + 1);
		}
	};

	_instructions[0x1d] = {
		"DEC E",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			uint8_t				e;

			e = regs->getE();
			regs->setFz(((e - 1) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_SUB(e, -1));
			regs->setE(regs->getE() - 1);
		}
	};

	_instructions[0x1e] = {
		"LD E,d8",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setE(mem->getByteAt(regs->getPC() + 1));
		}
	};

	_instructions[0x1f] = {
		"RRA",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			uint8_t				a;

			a = regs->getA();
			regs->setA(a >> 1 | (regs->getFc() << 7));
			regs->setFz(false);
			regs->setFn(false);
			regs->setFh(false);
			regs->setFc(a << 7);
		}
	};

	_instructions[0x20] = {
		"JR NZ,r8",
		2,
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			if (!regs->getFz())
				regs->setPC(regs->getPC() + static_cast<int8_t>(mem->getByteAt(regs->getPC() + 1)));
		}
	};

	_instructions[0x21] = {
		"LD HL,d16",
		3,
		12,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setH(mem->getByteAt(regs->getPC() + 1));
			regs->setL(mem->getByteAt(regs->getPC() + 2));
		}
	};

	_instructions[0x22] = {
		"LD (HL+),A",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, regs->getA());
			regs->setHL(hl + 1);
		}
	};

	_instructions[0x23] = {
		"INC HL",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setHL(regs->getHL() + 1);
		}
	};

	_instructions[0x24] = {
		"INC H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		h;

			h = regs->getH();
			regs->setFz(((h + 1) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_ADD(h, 1));
			regs->setFc(FLAG_C8_ADD(h, 1));
			regs->setH(h + 1);
		}
	};

	_instructions[0x25] = {
		"DEC H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		h;

			h = regs->getH();
			regs->setFz(((h - 1) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_SUB(h, 1));
			regs->setFc(FLAG_C8_SUB(h, 1));
			regs->setH(h - 1);
		}
	};

	_instructions[0x26] = {
		"LD H,d8",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setH(mem->getByteAt(regs->getPC() + 1));
		}
	};

	/**
	 * --------------------------------------------------------------------------------
	 * |           | C Flag  | HEX value in | H Flag | HEX value in | Number  | C flag|
	 * | Operation | Before  | upper digit  | Before | lower digit  | added   | After |
	 * |           | DAA     | (bit 7-4)    | DAA    | (bit 3-0)    | to byte | DAA   |
	 * |------------------------------------------------------------------------------|
	 * |           |    0    |     0-9      |   0    |     0-9      |   00    |   0   | (0) NI
	 * |           |    0    |     0-8      |   0    |     A-F      |   06    |   0   | (1)
	 * |   ADD     |    0    |     0-9      |   1    |     0-3      |   06    |   0   | (2)
	 * |   ADC     |    0    |     A-F      |   0    |     0-9      |   60    |   1   | (3)
	 * |  (N=0)    |    0    |     9-F      |   0    |     A-F      |   66    |   1   | (4)
	 * |           |    0    |     A-F      |   1    |     0-3      |   66    |   1   | (5)
	 * |           |    1    |     0-2      |   0    |     0-9      |   60    |   1   | (6)
	 * |           |    1    |     0-2      |   0    |     A-F      |   66    |   1   | (7)
	 * |           |    1    |     0-3      |   1    |     0-3      |   66    |   1   | (8)
	 * |------------------------------------------------------------------------------|
	 * |   SUB     |    0    |     0-9      |   0    |     0-9      |   00    |   0   | (9)  NI
	 * |   SBC     |    0    |     0-8      |   1    |     6-F      |   FA    |   0   | (10)
	 * |  (N=1)    |    1    |     7-F      |   0    |     0-9      |   A0    |   1   | (11)
	 * |           |    1    |     6-F      |   1    |     6-F      |   9A    |   1   | (12)
	 * |------------------------------------------------------------------------------|
	 *																		NI = not implemented
	 * See also: http://www.z80.info/zip/z80-documented.pdf (p.18)
	 */
	_instructions[0x27] = {
		"DAA",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		a, hi, lo;

			a = regs->getA();
			hi = a & 0xF0;
			lo = a & 0x0F;
			if (!regs->getFn()) { // if previous instruction was ADD/ADC
				if (!regs->getFc() && hi <= 8 && !regs->getFh() && lo >= 0xA) {			// (1)
					regs->setA(a + 0x06);
					regs->setFc(false);
				}
				else if (!regs->getFc() && hi <= 9 && regs->getFh() && lo <= 3) {		// (2)
					regs->setA(a + 0x06);
					regs->setFc(false);
				}
				else if (!regs->getFc() && hi >= 0xA && !regs->getFh() && lo <= 9) {	// (3)
					regs->setA(a + 0x60);
					regs->setFc(true);
				}
				else if (!regs->getFc() && hi >= 9 && !regs->getFh() && lo >= 0xA) {	// (4)
					regs->setA(a + 0x66);
					regs->setFc(true);
				}
				else if (!regs->getFc() && hi >= 0xA && regs->getFh() && lo <= 3) {		// (5)
					regs->setA(a + 0x66);
					regs->setFc(true);
				}
				else if (regs->getFc() && hi <= 2 && !regs->getFh() && lo <= 9) {		// (6)
					regs->setA(a + 0x60);
					regs->setFc(true);
				}
				else if (regs->getFc() && hi <= 2 && !regs->getFh() && lo >= 0xA) {		// (7)
					regs->setA(a + 0x66);
					regs->setFc(true);
				}
				else if (regs->getFc() && hi <= 3 && regs->getFh() && lo <= 3) {		// (8)
					regs->setA(a + 0x66);
					regs->setFc(true);
				}
			} else {	// if previous instruction was a SUB/SBC
				if (!regs->getFc() && hi <= 8 && regs->getFh() && lo >= 6) {			// (10)
					regs->setA(a + 0xfa);
					regs->setFc(false);
				}
				else if (regs->getFc() && hi >= 7 && !regs->getFh() && lo <= 9) {		// (11)
					regs->setA(a + 0xa0);
					regs->setFc(true);
				}
				else if (regs->getFc() && hi >= 6 && regs->getFh() && lo >= 6) {		// (12)
					regs->setA(a + 0x9a);
					regs->setFc(true);
				}
			}
			regs->setFz(regs->getA() == 0);
			regs->setFh(false);
		}
	};

	_instructions[0x28] = {
		"JR Z,r8",
		2,
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			if (regs->getFz()) {
				regs->setPC(static_cast<int8_t>(mem->getByteAt(regs->getPC() + 1)));
				// cycles += 4
			}
		}
	};

	_instructions[0x29] = {
		"ADD HL,HL",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint16_t		hl;

			hl = regs->getHL();
			regs->setHL(hl + hl);
			regs->setFn(false);
			regs->setFh(FLAG_H16_ADD(hl, hl));
			regs->setFc(FLAG_C16_ADD(hl, hl));
		}
	};

	_instructions[0x2a] = {
		"LD A,(HL+)",
		1,
		8,
		[](Cpu *cpu) {
			static Memory		*mem = cpu->memory();
			static Registers	*regs = cpu->regs();
			static uint16_t		hl;

			hl = regs->getHL();
			regs->setA(mem->getByteAt(hl));
			regs->setHL(hl + 1);
		}
	};

	_instructions[0x2b] = {
		"DEC HL",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setHL(regs->getHL() - 1);
		}
	};

	_instructions[0x2c] = {
		"INC L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		l;

			l = regs->getL();
			regs->setFz(((l + 1) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_ADD(l, 1));
			regs->setL(l + 1);
		}
	};

	_instructions[0x2d] = {
		"DEC L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		l;

			l = regs->getL();
			regs->setFz(((l - 1) & 0xff) == 0);
			regs->setFn(true);
			regs->setFh(FLAG_H8_SUB(l, 1));
			regs->setL(l - 1);
		}
	};

	_instructions[0x2e] = {
		"LD L,d8",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setL(mem->getByteAt(regs->getPC() + 1));
		}
	};

	_instructions[0x2f] = {
		"CPL",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(~regs->getA());
			regs->setFn(true);
			regs->setFh(true);
		}
	};

	_instructions[0x30] = {
		"JR NC,r8",
		2,
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			if (!regs->getFc()) {
				regs->setPC(static_cast<int8_t>(mem->getByteAt(regs->getPC() + 1)));
				// cycles += 2
			}
		}
	};

	_instructions[0x31] = {
		"LD SP,d16",
		3,
		12,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		pc;

			pc = regs->getPC();
			regs->setSP(mem->getByteAt(pc + 1) | (mem->getByteAt(pc + 2) << 8));
		}
	};

	_instructions[0x32] = {
		"LD (HL-),A",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, regs->getA());
			regs->setHL(hl - 1);
		}
	};

	_instructions[0x33] = {
		"INC SP",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setSP(regs->getSP() + 1);
		}
	};

	_instructions[0x34] = {
		"INC (HL)",
		1,
		12,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		value, hl;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			regs->setFz(((value + 1) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_ADD(value, 1));
			mem->setByteAt(hl, value + 1);
		}
	};

	_instructions[0x35] = {
		"DEC (HL)",
		1,
		12,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		value, hl;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			regs->setFz(((value - 1) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_SUB(value, 1));
			mem->setByteAt(hl, value - 1);
		}
	};

	_instructions[0x36] = {
		"LD (HL),d8",
		2,
		12,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			mem->setByteAt(regs->getHL(), mem->getByteAt(regs->getPC() + 1));
		}
	};

	_instructions[0x37] = {
		"SCF",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setFn(false);
			regs->setFh(false);
			regs->setFc(true);
		}
	};

	_instructions[0x38] = {
		"JR C,r8",
		2,
		8, // 12 if jump is taken
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			if (regs->getFc()) {
				regs->setPC(static_cast<int8_t>(mem->getByteAt(regs->getPC() + 1)));
			}
		}
	};

	_instructions[0x39] = {
		"ADD HL,SP",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint16_t		hl, sp;

			hl = regs->getHL();
			sp = regs->getSP();
			regs->setFn(false);
			regs->setFh(FLAG_H16_ADD(hl, sp));
			regs->setFc(FLAG_C16_ADD(hl, sp));
			regs->setHL(hl + sp);
		}
	};

	_instructions[0x3a] = {
		"LD A,(HL-)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			regs->setA(mem->getByteAt(hl));
			regs->setHL(hl - 1);
		}
	};

	_instructions[0x3b] = {
		"DEC SP",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setSP(regs->getSP() - 1);
		}
	};

	_instructions[0x3c] = {
		"INC A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		a;

			a = regs->getA();
			regs->setFz(((a + 1) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_ADD(a, 1));
			regs->setA(regs->getA() + 1);
		}
	};

	_instructions[0x3d] = {
		"DEC A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		a;

			a = regs->getA();
			regs->setFz(((a - 1) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_SUB(a, 1));
			regs->setA(regs->getA() - 1);
		}
	};

	_instructions[0x3e] = {
		"LD A,d8",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			regs->setA(mem->getByteAt(regs->getPC() + 1));
		}
	};

	_instructions[0x3f] = {
		"CCF",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setFn(false);
			regs->setFh(false);
			regs->setFc(~regs->getFc());
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

			regs->setL(mem->getByteAt(regs->getHL()));
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

			mem->setByteAt(regs->getHL(), regs->getE());
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
			if (cpu->onHalt() == true)
				cpu->setHALT(false);
			else
				cpu->setHALT(true);
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
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static uint8_t		a;
			static uint8_t		b;

			a = regs->getA();
			b = regs->getB();
			regs->setFz(((a + b) & 0xff) == 0);
			regs->setFn(false);
			regs->setFh(FLAG_H8_ADD(a, b));
			regs->setFh(FLAG_C8_ADD(a, b));
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
			static Registers	*regs = cpu->regs();
			AND(regs->getB(), cpu);
		}
	};

	_instructions[0xa1] = {
		"AND C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			AND(regs->getC(), cpu);
		}
	};

	_instructions[0xa2] = {
		"AND D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			AND(regs->getD(), cpu);
		}
	};

	_instructions[0xa3] = {
		"AND E",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			AND(regs->getE(), cpu);
		}
	};

	_instructions[0xa4] = {
		"AND H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			AND(regs->getH(), cpu);
		}
	};

	_instructions[0xa5] = {
		"AND L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			AND(regs->getL(), cpu);
		}
	};

	_instructions[0xa6] = {
		"AND (HL)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			AND(mem->getByteAt(regs->getHL()), cpu);
		}
	};

	_instructions[0xa7] = {
		"AND A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			AND(regs->getA(), cpu);
		}
	};

	_instructions[0xa8] = {
		"XOR B",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			XOR(regs->getB(), cpu);
		}
	};

	_instructions[0xa9] = {
		"XOR C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			XOR(regs->getC(), cpu);
		}
	};

	_instructions[0xaa] = {
		"XOR D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			XOR(regs->getD(), cpu);
		}
	};

	_instructions[0xab] = {
		"XOR E",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			XOR(regs->getE(), cpu);
		}
	};

	_instructions[0xac] = {
		"XOR H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			XOR(regs->getH(), cpu);
		}
	};

	_instructions[0xad] = {
		"XOR L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			XOR(regs->getL(), cpu);
		}
	};

	_instructions[0xae] = {
		"XOR (HL)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			XOR(mem->getByteAt(regs->getHL()), cpu);
		}
	};

	_instructions[0xaf] = {
		"XOR A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			XOR(regs->getA(), cpu);
		}
	};

	_instructions[0xb0] = {
		"OR B",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			OR(regs->getB(), cpu);
		}
	};

	_instructions[0xb1] = {
		"OR C",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			OR(regs->getC(), cpu);
		}
	};

	_instructions[0xb2] = {
		"OR D",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			OR(regs->getD(), cpu);
		}
	};

	_instructions[0xb3] = {
		"OR E",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			OR(regs->getE(), cpu);
		}
	};

	_instructions[0xb4] = {
		"OR H",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			OR(regs->getH(), cpu);
		}
	};

	_instructions[0xb5] = {
		"OR L",
		1,
		4,
		[](Cpu *cpu) {
			static Registers			*regs = cpu->regs();
			OR(regs->getH(), cpu);
		}
	};

	_instructions[0xb6] = {
		"OR (HL)",
		1,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			OR(mem->getByteAt(regs->getHL()), cpu);
		}
	};

	_instructions[0xb7] = {
		"OR A",
		1,
		4,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			OR(regs->getA(), cpu);
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

	_cbInstructions[0x0] = {
		"RLC B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(CB_RLC(regs->getB(), cpu));
		}
	};

	_cbInstructions[0x1] = {
		"RLC C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(CB_RLC(regs->getC(), cpu));
		}
	};

	_cbInstructions[0x2] = {
		"RLC D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(CB_RLC(regs->getD(), cpu));
		}
	};

	_cbInstructions[0x3] = {
		"RLC E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(CB_RLC(regs->getE(), cpu));
		}
	};

	_cbInstructions[0x4] = {
		"RLC H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(CB_RLC(regs->getH(), cpu));
		}
	};

	_cbInstructions[0x5] = {
		"RLC L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(CB_RLC(regs->getL(), cpu));
		}
	};

	_cbInstructions[0x6] = {
		"RLC (HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			mem->setByteAt(hl, CB_RLC(value, cpu));
		}
	};

	_cbInstructions[0x7] = {
		"RLC A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(CB_RLC(regs->getA(), cpu));
		}
	};

	_cbInstructions[0x8] = {
		"RRC B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(CB_RRC(regs->getB(), cpu));
		}
	};

	_cbInstructions[0x9] = {
		"RRC C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(CB_RRC(regs->getC(), cpu));
		}
	};

	_cbInstructions[0xa] = {
		"RRC D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(CB_RRC(regs->getD(), cpu));
		}
	};

	_cbInstructions[0xb] = {
		"RRC E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(CB_RRC(regs->getE(), cpu));
		}
	};

	_cbInstructions[0xc] = {
		"RRC H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(CB_RRC(regs->getH(), cpu));
		}
	};

	_cbInstructions[0xd] = {
		"RRC L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(CB_RRC(regs->getL(), cpu));
		}
	};

	_cbInstructions[0xe] = {
		"RRC (HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			mem->setByteAt(hl, CB_RRC(value, cpu));
		}
	};

	_cbInstructions[0xf] = {
		"RRC A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(CB_RRC(regs->getA(), cpu));
		}
	};

	_cbInstructions[0x10] = {
		"RL B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(CB_RL(regs->getB(), cpu));
		}
	};

	_cbInstructions[0x11] = {
		"RL C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(CB_RL(regs->getC(), cpu));
		}
	};

	_cbInstructions[0x12] = {
		"RL D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(CB_RL(regs->getD(), cpu));
		}
	};

	_cbInstructions[0x13] = {
		"RL E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(CB_RL(regs->getE(), cpu));
		}
	};

	_cbInstructions[0x14] = {
		"RL H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(CB_RL(regs->getH(), cpu));
		}
	};

	_cbInstructions[0x15] = {
		"RL L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(CB_RL(regs->getL(), cpu));
		}
	};

	_cbInstructions[0x16] = {
		"RL (HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			mem->setByteAt(hl, CB_RL(value, cpu));
		}
	};

	_cbInstructions[0x17] = {
		"RL A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(CB_RL(regs->getA(), cpu));
		}
	};

	_cbInstructions[0x18] = {
		"RR B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(CB_RR(regs->getB(), cpu));
		}
	};

	_cbInstructions[0x19] = {
		"RR C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(CB_RR(regs->getC(), cpu));
		}
	};

	_cbInstructions[0x1a] = {
		"RR D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(CB_RR(regs->getD(), cpu));
		}
	};

	_cbInstructions[0x1b] = {
		"RR E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(CB_RR(regs->getE(), cpu));
		}
	};

	_cbInstructions[0x1c] = {
		"RR H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(CB_RR(regs->getH(), cpu));
		}
	};

	_cbInstructions[0x1d] = {
		"RR L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(CB_RR(regs->getL(), cpu));
		}
	};

	_cbInstructions[0x1e] = {
		"RR (HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			mem->setByteAt(hl, CB_RR(value, cpu));
		}
	};

	_cbInstructions[0x1f] = {
		"RR A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(CB_RR(regs->getA(), cpu));
		}
	};

	_cbInstructions[0x20] = {
		"SLA B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(CB_SLA(regs->getB(), cpu));
		}
	};

	_cbInstructions[0x21] = {
		"SLA C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(CB_SLA(regs->getC(), cpu));
		}
	};

	_cbInstructions[0x22] = {
		"SLA D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(CB_SLA(regs->getD(), cpu));
		}
	};

	_cbInstructions[0x23] = {
		"SLA E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(CB_SLA(regs->getE(), cpu));
		}
	};

	_cbInstructions[0x24] = {
		"SLA H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(CB_SLA(regs->getH(), cpu));
		}
	};

	_cbInstructions[0x25] = {
		"SLA L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(CB_SLA(regs->getL(), cpu));
		}
	};

	_cbInstructions[0x26] = {
		"SLA (HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			mem->setByteAt(hl, CB_SLA(value, cpu));
		}
	};

	_cbInstructions[0x27] = {
		"SLA A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(CB_SLA(regs->getA(), cpu));
		}
	};

	_cbInstructions[0x28] = {
		"SRA B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(CB_SRA(regs->getB(), cpu));
		}
	};

	_cbInstructions[0x29] = {
		"SRA C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(CB_SRA(regs->getC(), cpu));
		}
	};

	_cbInstructions[0x2a] = {
		"SRA D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(CB_SRA(regs->getD(), cpu));
		}
	};

	_cbInstructions[0x2b] = {
		"SRA E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(CB_SRA(regs->getE(), cpu));
		}
	};

	_cbInstructions[0x2c] = {
		"SRA H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(CB_SRA(regs->getH(), cpu));
		}
	};

	_cbInstructions[0x2d] = {
		"SRA L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(CB_SRA(regs->getL(), cpu));
		}
	};

	_cbInstructions[0x2e] = {
		"SRA (HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			mem->setByteAt(hl, CB_SRA(value, cpu));
		}
	};

	_cbInstructions[0x2f] = {
		"SRA A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(CB_SRA(regs->getA(), cpu));
		}
	};

	_cbInstructions[0x30] = {
		"SWAP B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(CB_SWAP(regs->getB(), cpu));
		}
	};

	_cbInstructions[0x31] = {
		"SWAP C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(CB_SWAP(regs->getC(), cpu));
		}
	};

	_cbInstructions[0x32] = {
		"SWAP D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(CB_SWAP(regs->getD(), cpu));
		}
	};

	_cbInstructions[0x33] = {
		"SWAP E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(CB_SWAP(regs->getE(), cpu));
		}
	};

	_cbInstructions[0x34] = {
		"SWAP H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(CB_SWAP(regs->getH(), cpu));
		}
	};

	_cbInstructions[0x35] = {
		"SWAP L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(CB_SWAP(regs->getL(), cpu));
		}
	};

	_cbInstructions[0x36] = {
		"SWAP (HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			mem->setByteAt(hl, CB_SWAP(value, cpu));
		}
	};

	_cbInstructions[0x37] = {
		"SWAP A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(CB_SWAP(regs->getA(), cpu));
		}
	};

	_cbInstructions[0x38] = {
		"SRL B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(CB_SRL(regs->getB(), cpu));
		}
	};

	_cbInstructions[0x39] = {
		"SRL C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(CB_SRL(regs->getC(), cpu));
		}
	};

	_cbInstructions[0x3a] = {
		"SRL D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(CB_SRL(regs->getD(), cpu));
		}
	};

	_cbInstructions[0x3b] = {
		"SRL E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(CB_SRL(regs->getE(), cpu));
		}
	};

	_cbInstructions[0x3c] = {
		"SRL H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(CB_SRL(regs->getH(), cpu));
		}
	};

	_cbInstructions[0x3d] = {
		"SRL L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(CB_SRL(regs->getL(), cpu));
		}
	};

	_cbInstructions[0x3e] = {
		"SRL (HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			mem->setByteAt(hl, CB_SRL(value, cpu));
		}
	};

	_cbInstructions[0x3f] = {
		"SRL A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(CB_SRL(regs->getA(), cpu));
		}
	};

	_cbInstructions[0x40] = {
		"BIT 0,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(0, regs->getB(), cpu);
		}
	};

	_cbInstructions[0x41] = {
		"BIT 0,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(0, regs->getC(), cpu);
		}
	};

	_cbInstructions[0x42] = {
		"BIT 0,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(0, regs->getD(), cpu);
		}
	};

	_cbInstructions[0x43] = {
		"BIT 0,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(0, regs->getE(), cpu);
		}
	};

	_cbInstructions[0x44] = {
		"BIT 0,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(0, regs->getH(), cpu);
		}
	};

	_cbInstructions[0x45] = {
		"BIT 0,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(0, regs->getL(), cpu);
		}
	};

	_cbInstructions[0x46] = {
		"BIT 0,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();

			CB_BIT(0, mem->getByteAt(regs->getHL()), cpu);
		}
	};

	_cbInstructions[0x47] = {
		"BIT 0,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(0, regs->getA(), cpu);
		}
	};

	_cbInstructions[0x48] = {
		"BIT 1,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(1, regs->getB(), cpu);
		}
	};

	_cbInstructions[0x49] = {
		"BIT 1,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(1, regs->getC(), cpu);
		}
	};

	_cbInstructions[0x4a] = {
		"BIT 1,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(1, regs->getD(), cpu);
		}
	};

	_cbInstructions[0x4b] = {
		"BIT 1,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(1, regs->getE(), cpu);
		}
	};

	_cbInstructions[0x4c] = {
		"BIT 1,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(1, regs->getH(), cpu);
		}
	};

	_cbInstructions[0x4d] = {
		"BIT 1,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(1, regs->getL(), cpu);
		}
	};

	_cbInstructions[0x4e] = {
		"BIT 1,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			CB_BIT(1, value, cpu);
		}
	};

	_cbInstructions[0x4f] = {
		"BIT 1,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(1, regs->getA(), cpu);
		}
	};

	_cbInstructions[0x50] = {
		"BIT 2,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(2, regs->getB(), cpu);
		}
	};

	_cbInstructions[0x51] = {
		"BIT 2,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(2, regs->getC(), cpu);
		}
	};

	_cbInstructions[0x52] = {
		"BIT 2,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(2, regs->getD(), cpu);
		}
	};

	_cbInstructions[0x53] = {
		"BIT 2,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(2, regs->getE(), cpu);
		}
	};

	_cbInstructions[0x54] = {
		"BIT 2,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(2, regs->getH(), cpu);
		}
	};

	_cbInstructions[0x55] = {
		"BIT 2,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(2, regs->getL(), cpu);
		}
	};

	_cbInstructions[0x56] = {
		"BIT 2,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			CB_BIT(2, value, cpu);
		}
	};

	_cbInstructions[0x57] = {
		"BIT 2,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(2, regs->getA(), cpu);
		}
	};

	_cbInstructions[0x58] = {
		"BIT 3,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(3, regs->getB(), cpu);
		}
	};

	_cbInstructions[0x59] = {
		"BIT 3,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(3, regs->getC(), cpu);
		}
	};

	_cbInstructions[0x5a] = {
		"BIT 3,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(3, regs->getD(), cpu);
		}
	};

	_cbInstructions[0x5b] = {
		"BIT 3,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(3, regs->getE(), cpu);
		}
	};

	_cbInstructions[0x5c] = {
		"BIT 3,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(3, regs->getH(), cpu);
		}
	};

	_cbInstructions[0x5d] = {
		"BIT 3,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(3, regs->getL(), cpu);
		}
	};

	_cbInstructions[0x5e] = {
		"BIT 3,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			CB_BIT(3, value, cpu);
		}
	};

	_cbInstructions[0x5f] = {
		"BIT 3,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(3, regs->getA(), cpu);
		}
	};

	_cbInstructions[0x60] = {
		"BIT 4,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(4, regs->getB(), cpu);
		}
	};

	_cbInstructions[0x61] = {
		"BIT 4,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(4, regs->getC(), cpu);
		}
	};

	_cbInstructions[0x62] = {
		"BIT 4,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(4, regs->getD(), cpu);
		}
	};

	_cbInstructions[0x63] = {
		"BIT 4,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(4, regs->getE(), cpu);
		}
	};

	_cbInstructions[0x64] = {
		"BIT 4,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(4, regs->getH(), cpu);
		}
	};

	_cbInstructions[0x65] = {
		"BIT 4,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(4, regs->getL(), cpu);
		}
	};

	_cbInstructions[0x66] = {
		"BIT 4,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			CB_BIT(4, value, cpu);
		}
	};

	_cbInstructions[0x67] = {
		"BIT 4,A",
		2,
		8,
		[](Cpu *cpu) {
			(void)cpu;
			static Registers	*regs = cpu->regs();

			CB_BIT(4, regs->getA(), cpu);
		}
	};

	_cbInstructions[0x68] = {
		"BIT 5,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(5, regs->getB(), cpu);
		}
	};

	_cbInstructions[0x69] = {
		"BIT 5,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(5, regs->getC(), cpu);
		}
	};

	_cbInstructions[0x6a] = {
		"BIT 5,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(5, regs->getD(), cpu);
		}
	};

	_cbInstructions[0x6b] = {
		"BIT 5,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(5, regs->getE(), cpu);
		}
	};

	_cbInstructions[0x6c] = {
		"BIT 5,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(5, regs->getH(), cpu);
		}
	};

	_cbInstructions[0x6d] = {
		"BIT 5,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(5, regs->getL(), cpu);
		}
	};

	_cbInstructions[0x6e] = {
		"BIT 5,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			CB_BIT(5, value, cpu);
		}
	};

	_cbInstructions[0x6f] = {
		"BIT 5,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(5, regs->getA(), cpu);
		}
	};

	_cbInstructions[0x70] = {
		"BIT 6,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(6, regs->getB(), cpu);
		}
	};

	_cbInstructions[0x71] = {
		"BIT 6,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(6, regs->getC(), cpu);
		}
	};

	_cbInstructions[0x72] = {
		"BIT 6,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(6, regs->getD(), cpu);
		}
	};

	_cbInstructions[0x73] = {
		"BIT 6,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(6, regs->getE(), cpu);
		}
	};

	_cbInstructions[0x74] = {
		"BIT 6,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(6, regs->getH(), cpu);
		}
	};

	_cbInstructions[0x75] = {
		"BIT 6,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(6, regs->getL(), cpu);
		}
	};

	_cbInstructions[0x76] = {
		"BIT 6,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			CB_BIT(6, value, cpu);
		}
	};

	_cbInstructions[0x78] = {
		"BIT 6,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(6, regs->getA(), cpu);
		}
	};

	_cbInstructions[0x78] = {
		"BIT 7,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(7, regs->getB(), cpu);
		}
	};

	_cbInstructions[0x79] = {
		"BIT 7,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(7, regs->getC(), cpu);
		}
	};

	_cbInstructions[0x7a] = {
		"BIT 7,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(7, regs->getD(), cpu);
		}
	};

	_cbInstructions[0x7b] = {
		"BIT 7,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(7, regs->getE(), cpu);
		}
	};

	_cbInstructions[0x7c] = {
		"BIT 7,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(7, regs->getH(), cpu);
		}
	};

	_cbInstructions[0x7d] = {
		"BIT 7,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(7, regs->getL(), cpu);
		}
	};

	_cbInstructions[0x7e] = {
		"BIT 7,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;
			static uint8_t		value;

			hl = regs->getHL();
			value = mem->getByteAt(hl);
			CB_BIT(7, value, cpu);
		}
	};

	_cbInstructions[0x7f] = {
		"BIT 7,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			CB_BIT(7, regs->getA(), cpu);
		}
	};

	_cbInstructions[0x80] = {
		"RES 0,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() & ~(1 << 0));
		}
	};

	_cbInstructions[0x81] = {
		"RES 0,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() & ~(1 << 0));
		}
	};

	_cbInstructions[0x82] = {
		"RES 0,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() & ~(1 << 0));
		}
	};

	_cbInstructions[0x83] = {
		"RES 0,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() & ~(1 << 0));
		}
	};

	_cbInstructions[0x84] = {
		"RES 0,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() & ~(1 << 0));
		}
	};

	_cbInstructions[0x85] = {
		"RES 0,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() & ~(1 << 0));
		}
	};

	_cbInstructions[0x86] = {
		"RES 0,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) & ~(1 << 0));
		}
	};

	_cbInstructions[0x87] = {
		"RES 0,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() & ~(1 << 0));
		}
	};

	_cbInstructions[0x88] = {
		"RES 1,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() & ~(1 << 1));
		}
	};

	_cbInstructions[0x89] = {
		"RES 1,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() & ~(1 << 1));
		}
	};

	_cbInstructions[0x8a] = {
		"RES 1,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() & ~(1 << 1));
		}
	};

	_cbInstructions[0x8b] = {
		"RES 1,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() & ~(1 << 1));
		}
	};

	_cbInstructions[0x8c] = {
		"RES 1,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() & ~(1 << 1));
		}
	};

	_cbInstructions[0x8d] = {
		"RES 1,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() & ~(1 << 1));
		}
	};

	_cbInstructions[0x8e] = {
		"RES 1,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) & ~(1 << 1));
		}
	};

	_cbInstructions[0x8f] = {
		"RES 1,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() & ~(1 << 1));
		}
	};

	_cbInstructions[0x90] = {
		"RES 2,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() & ~(1 << 2));
		}
	};

	_cbInstructions[0x91] = {
		"RES 2,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() & ~(1 << 2));
		}
	};

	_cbInstructions[0x92] = {
		"RES 2,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() & ~(1 << 2));
		}
	};

	_cbInstructions[0x93] = {
		"RES 2,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() & ~(1 << 2));
		}
	};

	_cbInstructions[0x94] = {
		"RES 2,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() & ~(1 << 2));
		}
	};

	_cbInstructions[0x95] = {
		"RES 2,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() & ~(1 << 2));
		}
	};

	_cbInstructions[0x96] = {
		"RES 2,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) & ~(1 << 2));
		}
	};

	_cbInstructions[0x97] = {
		"RES 2,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() & ~(1 << 2));
		}
	};

	_cbInstructions[0x98] = {
		"RES 3,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() & ~(1 << 3));
		}
	};

	_cbInstructions[0x99] = {
		"RES 3,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() & ~(1 << 3));
		}
	};

	_cbInstructions[0x9a] = {
		"RES 3,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() & ~(1 << 3));
		}
	};

	_cbInstructions[0x9b] = {
		"RES 3,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() & ~(1 << 3));
		}
	};

	_cbInstructions[0x9c] = {
		"RES 3,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() & ~(1 << 3));
		}
	};

	_cbInstructions[0x9d] = {
		"RES 3,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() & ~(1 << 3));
		}
	};

	_cbInstructions[0x9e] = {
		"RES 3,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) & ~(1 << 3));
		}
	};

	_cbInstructions[0x9f] = {
		"RES 3,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() & ~(1 << 3));
		}
	};

	_cbInstructions[0xa0] = {
		"RES 4,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() & ~(1 << 4));
		}
	};

	_cbInstructions[0xa1] = {
		"RES 4,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() & ~(1 << 4));
		}
	};

	_cbInstructions[0xa2] = {
		"RES 4,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() & ~(1 << 4));
		}
	};

	_cbInstructions[0xa3] = {
		"RES 4,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() & ~(1 << 4));
		}
	};

	_cbInstructions[0xa4] = {
		"RES 4,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() & ~(1 << 4));
		}
	};

	_cbInstructions[0xa5] = {
		"RES 4,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() & ~(1 << 4));
		}
	};

	_cbInstructions[0xa6] = {
		"RES 4,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) & ~(1 << 4));
		}
	};

	_cbInstructions[0xa7] = {
		"RES 4,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() & ~(1 << 4));
		}
	};

	_cbInstructions[0xa8] = {
		"RES 5,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() & ~(1 << 5));
		}
	};

	_cbInstructions[0xa9] = {
		"RES 5,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() & ~(1 << 5));
		}
	};

	_cbInstructions[0xaa] = {
		"RES 5,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() & ~(1 << 5));
		}
	};

	_cbInstructions[0xab] = {
		"RES 5,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() & ~(1 << 5));
		}
	};

	_cbInstructions[0xac] = {
		"RES 5,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() & ~(1 << 5));
		}
	};

	_cbInstructions[0xad] = {
		"RES 5,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() & ~(1 << 5));
		}
	};

	_cbInstructions[0xae] = {
		"RES 5,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) & ~(1 << 5));
		}
	};

	_cbInstructions[0xaf] = {
		"RES 5,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() & ~(1 << 5));
		}
	};

	_cbInstructions[0xb0] = {
		"RES 6,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() & ~(1 << 6));
		}
	};

	_cbInstructions[0xb1] = {
		"RES 6,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() & ~(1 << 6));
		}
	};

	_cbInstructions[0xb2] = {
		"RES 6,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() & ~(1 << 6));
		}
	};

	_cbInstructions[0xb3] = {
		"RES 6,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() & ~(1 << 6));
		}
	};

	_cbInstructions[0xb4] = {
		"RES 6,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() & ~(1 << 6));
		}
	};

	_cbInstructions[0xb5] = {
		"RES 6,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() & ~(1 << 6));
		}
	};

	_cbInstructions[0xb6] = {
		"RES 6,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) & ~(1 << 6));
		}
	};

	_cbInstructions[0xb8] = {
		"RES 6,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() & ~(1 << 6));
		}
	};

	_cbInstructions[0xb8] = {
		"RES 7,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() & ~(1 << 7));
		}
	};

	_cbInstructions[0xb9] = {
		"RES 7,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() & ~(1 << 7));
		}
	};

	_cbInstructions[0xba] = {
		"RES 7,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() & ~(1 << 7));
		}
	};

	_cbInstructions[0xbb] = {
		"RES 7,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() & ~(1 << 7));
		}
	};

	_cbInstructions[0xbc] = {
		"RES 7,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() & ~(1 << 7));
		}
	};

	_cbInstructions[0xbd] = {
		"RES 7,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() & ~(1 << 7));
		}
	};

	_cbInstructions[0xbe] = {
		"RES 7,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) & ~(1 << 7));
		}
	};

	_cbInstructions[0xbf] = {
		"RES 7,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() & ~(1 << 7));
		}
	};


	_cbInstructions[0xc0] = {
		"SET 0,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() | (1 << 0));
		}
	};

	_cbInstructions[0xc1] = {
		"SET 0,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() | (1 << 0));
		}
	};

	_cbInstructions[0xc2] = {
		"SET 0,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() | (1 << 0));
		}
	};

	_cbInstructions[0xc3] = {
		"SET 0,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() | (1 << 0));
		}
	};

	_cbInstructions[0xc4] = {
		"SET 0,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() | (1 << 0));
		}
	};

	_cbInstructions[0xc5] = {
		"SET 0,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() | (1 << 0));
		}
	};

	_cbInstructions[0xc6] = {
		"SET 0,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) | (1 << 0));
		}
	};

	_cbInstructions[0xc7] = {
		"SET 0,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() | (1 << 0));
		}
	};

	_cbInstructions[0xc8] = {
		"SET 1,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() | (1 << 1));
		}
	};

	_cbInstructions[0xc9] = {
		"SET 1,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() | (1 << 1));
		}
	};

	_cbInstructions[0xca] = {
		"SET 1,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() | (1 << 1));
		}
	};

	_cbInstructions[0xcb] = {
		"SET 1,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() | (1 << 1));
		}
	};

	_cbInstructions[0xcc] = {
		"SET 1,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() | (1 << 1));
		}
	};

	_cbInstructions[0xcd] = {
		"SET 1,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() | (1 << 1));
		}
	};

	_cbInstructions[0xce] = {
		"SET 1,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) | (1 << 1));
		}
	};

	_cbInstructions[0xcf] = {
		"SET 1,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() | (1 << 1));
		}
	};

	_cbInstructions[0xd0] = {
		"SET 2,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() | (1 << 2));
		}
	};

	_cbInstructions[0xd1] = {
		"SET 2,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() | (1 << 2));
		}
	};

	_cbInstructions[0xd2] = {
		"SET 2,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() | (1 << 2));
		}
	};

	_cbInstructions[0xd3] = {
		"SET 2,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() | (1 << 2));
		}
	};

	_cbInstructions[0xd4] = {
		"SET 2,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() | (1 << 2));
		}
	};

	_cbInstructions[0xd5] = {
		"SET 2,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() | (1 << 2));
		}
	};

	_cbInstructions[0xd6] = {
		"SET 2,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) | (1 << 2));
		}
	};

	_cbInstructions[0xd7] = {
		"SET 2,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() | (1 << 2));
		}
	};

	_cbInstructions[0xd8] = {
		"SET 3,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() | (1 << 3));
		}
	};

	_cbInstructions[0xd9] = {
		"SET 3,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() | (1 << 3));
		}
	};

	_cbInstructions[0xda] = {
		"SET 3,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() | (1 << 3));
		}
	};

	_cbInstructions[0xdb] = {
		"SET 3,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() | (1 << 3));
		}
	};

	_cbInstructions[0xdc] = {
		"SET 3,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() | (1 << 3));
		}
	};

	_cbInstructions[0xdd] = {
		"SET 3,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() | (1 << 3));
		}
	};

	_cbInstructions[0xde] = {
		"SET 3,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) | (1 << 3));
		}
	};

	_cbInstructions[0xdf] = {
		"SET 3,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() | (1 << 3));
		}
	};

	_cbInstructions[0xe0] = {
		"SET 4,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() | (1 << 4));
		}
	};

	_cbInstructions[0xe1] = {
		"SET 4,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() | (1 << 4));
		}
	};

	_cbInstructions[0xe2] = {
		"SET 4,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() | (1 << 4));
		}
	};

	_cbInstructions[0xe3] = {
		"SET 4,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() | (1 << 4));
		}
	};

	_cbInstructions[0xe4] = {
		"SET 4,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() | (1 << 4));
		}
	};

	_cbInstructions[0xe5] = {
		"SET 4,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() | (1 << 4));
		}
	};

	_cbInstructions[0xe6] = {
		"SET 4,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) | (1 << 4));
		}
	};

	_cbInstructions[0xe7] = {
		"SET 4,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() | (1 << 4));
		}
	};

	_cbInstructions[0xe8] = {
		"SET 5,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() | (1 << 5));
		}
	};

	_cbInstructions[0xe9] = {
		"SET 5,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() | (1 << 5));
		}
	};

	_cbInstructions[0xea] = {
		"SET 5,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() | (1 << 5));
		}
	};

	_cbInstructions[0xeb] = {
		"SET 5,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() | (1 << 5));
		}
	};

	_cbInstructions[0xec] = {
		"SET 5,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() | (1 << 5));
		}
	};

	_cbInstructions[0xed] = {
		"SET 5,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() | (1 << 5));
		}
	};

	_cbInstructions[0xee] = {
		"SET 5,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) | (1 << 5));
		}
	};

	_cbInstructions[0xef] = {
		"SET 5,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() | (1 << 5));
		}
	};

	_cbInstructions[0xf0] = {
		"SET 6,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() | (1 << 6));
		}
	};

	_cbInstructions[0xf1] = {
		"SET 6,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() | (1 << 6));
		}
	};

	_cbInstructions[0xf2] = {
		"SET 6,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() | (1 << 6));
		}
	};

	_cbInstructions[0xf3] = {
		"SET 6,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() | (1 << 6));
		}
	};

	_cbInstructions[0xf4] = {
		"SET 6,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() | (1 << 6));
		}
	};

	_cbInstructions[0xf5] = {
		"SET 6,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() | (1 << 6));
		}
	};

	_cbInstructions[0xf6] = {
		"SET 6,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) | (1 << 6));
		}
	};

	_cbInstructions[0xf8] = {
		"SET 6,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() | (1 << 6));
		}
	};

	_cbInstructions[0xf8] = {
		"SET 7,B",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setB(regs->getB() | (1 << 7));
		}
	};

	_cbInstructions[0xf9] = {
		"SET 7,C",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setC(regs->getC() | (1 << 7));
		}
	};

	_cbInstructions[0xfa] = {
		"SET 7,D",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setD(regs->getD() | (1 << 7));
		}
	};

	_cbInstructions[0xfb] = {
		"SET 7,E",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setE(regs->getE() | (1 << 7));
		}
	};

	_cbInstructions[0xfc] = {
		"SET 7,H",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setH(regs->getH() | (1 << 7));
		}
	};

	_cbInstructions[0xfd] = {
		"SET 7,L",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setL(regs->getL() | (1 << 7));
		}
	};

	_cbInstructions[0xfe] = {
		"SET 7,(HL)",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();
			static Memory		*mem = cpu->memory();
			static uint16_t		hl;

			hl = regs->getHL();
			mem->setByteAt(hl, mem->getByteAt(hl) | (1 << 7));
		}
	};

	_cbInstructions[0xff] = {
		"SET 7,A",
		2,
		8,
		[](Cpu *cpu) {
			static Registers	*regs = cpu->regs();

			regs->setA(regs->getA() | (1 << 7));
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

	instruction = &_instructions[opcode];				// get correct t_instruction structure
	instruction->exec(_cpu);							// execute instruction
	regs->setPC(regs->getPC() + instruction->size);		// add instruction size to current PC
}

/**
 * Subfunction for repetitive work
 */

void		Gbmu::Instructions::ADDA(uint8_t value, Cpu *cpu)
{
	static Registers	*regs = cpu->regs();
	uint8_t		a;
			
	a = regs->getA();
	regs->setFz(((a + value) & 0xff) == 0);
	regs->setFn(false);
	regs->setFh(FLAG_H8_ADD(a, value));
	regs->setFc(FLAG_C8_ADD(a, value));
	regs->setA(a + value);
}

void		Gbmu::Instructions::ADCA(uint8_t value, Cpu *cpu)
{
	static Registers	*regs = cpu->regs();
	
	value += regs->getFc();
	ADDA(value, cpu);
}

void		Gbmu::Instructions::SUBA(uint8_t value, Cpu *cpu)
{
	static Registers	*regs = cpu->regs();
	uint8_t		a;
			
	a = regs->getA();
	regs->setFz(((a - value) & 0xff) == 0);
	regs->setFn(true);
	regs->setFh(FLAG_H8_SUB(a, - value));
	regs->setFc(FLAG_C8_SUB(a, - value));
	regs->setA(a - value);
}

void		Gbmu::Instructions::SBCA(uint8_t value, Cpu *cpu)
{
	static Registers	*regs = cpu->regs();
	
	value += regs->getFc();
	SUBA(value, cpu);
}

void		Gbmu::Instructions::AND(uint8_t value, Cpu *cpu)
{
	static Registers	*regs = cpu->regs();

	regs->setA(regs->getA() & value);
	regs->setFz(regs->getA() ? 0 : 1); // set zero flags
	regs->setFn(0);
	regs->setFh(1);
	regs->setFc(0);
	// maybe to optimise by change just F.
}

void		Gbmu::Instructions::XOR(uint8_t value, Cpu *cpu)
{
	static Registers	*regs = cpu->regs();

	regs->setA(regs->getA() ^ value);
	regs->setFz(regs->getA() ? 0 : 1); // set zero flags
	regs->setFn(0);
	regs->setFh(0);
	regs->setFc(0);
	// maybe to optimise by change just F.
}

void		Gbmu::Instructions::OR(uint8_t value, Cpu *cpu)
{
	static Registers	*regs = cpu->regs();

	regs->setA(regs->getA() | value);
	regs->setFz(regs->getA() ? 0 : 1); // set zero flags
	regs->setFn(0);
	regs->setFh(0);
	regs->setFc(0);
}

void		Gbmu::Instructions::CP(uint8_t value, Cpu *cpu) //compare
{
	/**
	 * static gs are affected as follows
	 *
	 * C or carry flag          1 if <0 else 0
	 * Z or zero flag           1 if result = 0 else 0
	 * N flag                   1
	 * H or half carry flag     1 if borrow from bit 12 else 0
	 */
	static Registers	*regs = cpu->regs();

	regs->setFz(regs->getA() == value ? 1 : 0); // set zero flags
	regs->setFn(1);
	regs->setFh(0);
	regs->setFc(value > regs->getA() ? 1 : 0);
}

void		Gbmu::Instructions::RST(uint8_t value, Cpu *cpu) //compare
{
	static Registers	*regs = cpu->regs();
	static Memory		*mem = cpu->memory();

	mem->setByteAt((regs->getSP() - 1), (regs->getPC() & 0xff00) >> 8);
	mem->setByteAt((regs->getSP() - 2), (regs->getPC() & 0x00ff));
	regs->setSP(regs->getSP() - 2);
	regs->setPC(value);
}
uint8_t		Gbmu::Instructions::CB_RLC(uint8_t value, Cpu *cpu) {
	static Registers	*regs = cpu->regs();

	regs->setFz((((value << 1) | (value >> 7)) & 0xff) == 0);
	regs->setFn(false);
	regs->setFh(false);
	regs->setFc(value >> 7);
	return ((value << 1) | (value >> 7));
}

uint8_t		Gbmu::Instructions::CB_RRC(uint8_t value, Cpu *cpu) {
	static Registers	*regs = cpu->regs();

	regs->setFz((((value >> 1) | (value << 7)) & 0xff) == 0);
	regs->setFn(false);
	regs->setFh(false);
	regs->setFc(value & 1);
	return ((value >> 1) | (value << 7));
}

uint8_t		Gbmu::Instructions::CB_RL(uint8_t value, Cpu *cpu) {
	static Registers	*regs = cpu->regs();
	static uint8_t		retval;

	retval = (value << 1) | regs->getFc();
	regs->setFz((retval & 0xff) == 0);
	regs->setFn(false);
	regs->setFh(false);
	regs->setFc(value >> 7);
	return (retval);
}

uint8_t		Gbmu::Instructions::CB_RR(uint8_t value, Cpu *cpu) {
	static Registers	*regs = cpu->regs();
	static uint8_t		retval;

	retval = (value >> 1) | regs->getFc();
	regs->setFz((retval & 0xff) == 0);
	regs->setFh(false);
	regs->setFc(value << 7);
	return (retval);
}

uint8_t		Gbmu::Instructions::CB_SLA(uint8_t value, Cpu *cpu) {
	static Registers	*regs = cpu->regs();

	regs->setFz(((value << 1) & 0xff) == 0);
	regs->setFn(false);
	regs->setFh(false);
	regs->setFc(value >> 7);
	return (value << 1);
}

uint8_t		Gbmu::Instructions::CB_SRA(uint8_t value, Cpu *cpu) {
	static Registers	*regs = cpu->regs();
	static uint8_t		retval;

	retval = (value >> 1) | (value & (1 << 7));
	regs->setFz((retval & 0xff) == 0);
	regs->setFn(false);
	regs->setFh(false);
	regs->setFc(value >> 1);
	return ((value >> 1) | (value & (1 << 7)));
}

uint8_t		Gbmu::Instructions::CB_SWAP(uint8_t value, Cpu *cpu) {
	static Registers	*regs = cpu->regs();
	static uint8_t		retval;

	retval = ((value & 0x0f) << 4) | ((value & 0xf0) >> 4);
	regs->setFz((retval & 0xff) == 0);
	regs->setFn(false);
	regs->setFh(false);
	regs->setFc(false);
	return (retval);
}

uint8_t		Gbmu::Instructions::CB_SRL(uint8_t value, Cpu *cpu) {
	static Registers	*regs = cpu->regs();

	regs->setFz(((value >> 1) & 0xff) == 0);
	regs->setFn(false);
	regs->setFh(false);
	regs->setFc(value << 7);
	return (value >> 1);
}

void		Gbmu::Instructions::CB_BIT(int pos, uint8_t value, Cpu *cpu) {
	static Registers	*regs = cpu->regs();

	regs->setFn(false);
	regs->setFh(false);
	regs->setFz((value & (1 << pos)) == 0);
}
