
#ifndef CPU_CLASS_HPP
# define CPU_CLASS_HPP

# include <iostream>
# include <fstream>

# include "Memory.class.hpp"
# include "Cartridge.class.hpp"


namespace Gbmu{
	class Cpu
	{
		private:
			Memory*					_memory;			// GB General Memory
			Cartridge*				_cartridge;			// Cartridge
	//			bool					_BOOT;				// Booting Flag
	//			bool					_HALT;				// Halting Flag
	//			bool					_doubleSpeed;		// DoubleSpeed Flag (CGB ONLY)

		public:
			Cpu ( void );
			virtual ~Cpu ( void );
			Cpu(Cpu const & src);
			Cpu & operator=(Cpu const & rhs);

	/*NI*/		void					reset ( void );
	/*NI*/		void					loadCartridge ( std::string const& cartridgePath, Gb::Model const& model );
				
	/*NI*/		void					executeFrame ( void );
	/*NI*/		size_t					execute ( void );

	/*NI*/		void					onWriteKey1 ( uint8_t const& value );
	/*NI*/		void					switchSpeed ( void );

	/*NI*/		void					setHALT ( bool const& b );
	/*NI*/		void					stopBOOT ( void );

	/*NI*/		bool const&				onHalt ( void ) const;
	/*NI*/		bool const&				onBoot ( void ) const;

			Memory*					memory ( void ) const;
			Cartridge*				cartridge ( void ) const;

	/*NI*/		void					saveState ( std::fstream& file );
	/*NI*/		void					loadState ( std::fstream& file );
	};
}
#else
	namespace Gbmu
	{
		class Cpu;
	}
#endif // !CPU_CLASS_HPP
