#ifndef INSTRUCTIONS_CLASS_HPP
#define INSTRUCTIONS_CLASS_HPP

#include "Cpu.class.hpp"

#define FLAG_C8(X)			( ((X) >> 7) & 1 )
#define FLAG_C16(X, Y)		( ((X) >> 15) & 1 )

#define FLAG_H8(X, Y)		( (((X) & 0xf) + ((Y) & 0xf)) & 0x10 )
#define FLAG_H16(X, Y)		( (((X) & 0xff) + ((Y) & 0xff)) & 0x1000 )

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
			Instructions(void);
			Cpu				*_cpu;
			t_instruction	_instructions[0x100];
	};
}

#else

namespace Gbmu {
	class Instructions;
}

#endif /* end of include guard: INSTRUCTIONS_CLASS_HPP */
