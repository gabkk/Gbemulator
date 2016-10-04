#ifndef CPU_CLASS_HPP
# define CPU_CLASS_HPP

# include <iostream>
# include <fstream>
# include "Memory.class.hpp"
# include "Cartridge.class.hpp"
# include "Registers.class.hpp"

# define DEFAULT_PC 0x150
# define DEFAULT_SP 0xFFFE

namespace Gbmu{
	class Cpu
	{
		private:
			Registers		*_regs;			// general registers (A, B, ..)
			Memory			*_memory;		// gb memory
			Cartridge		*_cartridge;	// loaded cartridge
			uint16_t		_pc;			// program counter (address of the current instruction)
			uint16_t		_sp;			// stack pointer
			bool			_BOOT;			// Booting Flag
			bool			_HALT;			// Halting Flag
			//bool			_doubleSpeed;	// DoubleSpeed Flag (CGB ONLY)

		public:
			Cpu ( void );
			virtual ~Cpu ( void );

			/*NI*/		void		reset ( void );
			/*NI*/		void		loadCartridge ( std::string const& cartridgePath, Gb::Model const& model );

			void		executeFrame ( void );
			/*NI*/size_t		execute ( void );

			/*NI*/		void		onWriteKey1 ( uint8_t const& value );
			/*NI*/		void		switchSpeed ( void );

			/*NWI*/		void		setHALT ( bool const& b );
			/*NWI*/		void		stopBOOT ( void );

			bool const&				onHalt ( void ) const;
			bool const&				onBoot ( void ) const;

			Registers *				regs(void) const;
			Memory*					memory ( void ) const;
			Cartridge*				cartridge ( void ) const;

			/*NI*/		void		saveState ( std::fstream& file );
			/*NI*/		void		loadState ( std::fstream& file );
	};
}
#else
namespace Gbmu {
	class Cpu;
}
#endif // !CPU_CLASS_HPP
