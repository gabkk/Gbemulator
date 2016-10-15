#ifndef INSTRUCTIONS_CLASS_HPP
#define INSTRUCTIONS_CLASS_HPP

#include "Cpu.class.hpp"

#define FLAG_H8_ADD(X, Y)		( (((X) & 0xf) + ((Y) & 0xf)) & 0x10 )
#define FLAG_H8_SUB(X, Y)		( (((X) & 0xf) - ((Y) & 0xf)) & 0x10 )

#define FLAG_C8_ADD(X, Y)		( (((X) & 0xff) + ((Y) & 0xff)) & 0x100 )
#define FLAG_C8_SUB(X, Y)		( (((X) & 0xff) - ((Y) & 0xff)) & 0x100 )

#define FLAG_H16_ADD(X, Y)		( (((X) & 0xfff) + ((Y) & 0xfff)) & 0x1000 )
#define FLAG_H16_SUB(X, Y)		( (((X) & 0xfff) - ((Y) & 0xfff)) & 0x1000 )

#define FLAG_C16_ADD(X, Y)		( (((X) & 0xffff) + ((Y) & 0xffff)) & 0x10000 )
#define FLAG_C16_SUB(X, Y)		( (((X) & 0xffff) - ((Y) & 0xffff)) & 0x10000 )

namespace Gbmu {
	class Instructions {
		typedef struct		s_instruction {
			std::string		name;
			int				size;
			int				cycles;
			void			(*exec)(Cpu *cpu);
		}					t_instruction;

		public:
		Instructions(Cpu *cpu);
		virtual		~Instructions(void);
		void		execute(uint8_t opcode);

		private:
		Instructions(void);								// fordib instanciation without Cpu
		Cpu				*_cpu;
		t_instruction	_instructions[0x100];
		static void		AND(uint8_t value, Cpu *cpu);
		static void		XOR(uint8_t value, Cpu *cpu);
		static void		OR(uint8_t value, Cpu *cpu);
	};
}

#else

namespace Gbmu {
	class Instructions;
}

#endif /* end of include guard: INSTRUCTIONS_CLASS_HPP */
