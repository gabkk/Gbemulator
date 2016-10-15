#ifndef INSTRUCTIONS_CLASS_HPP
#define INSTRUCTIONS_CLASS_HPP

#include "Cpu.class.hpp"

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

#endif 