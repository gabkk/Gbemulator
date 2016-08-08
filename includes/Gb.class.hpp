#ifndef GB_CLASS_HPP
# define GB_CLASS_HPP

/*
** This class represent the game boy object
*/

# include <iostream>
# include <fstream>
# include <string>
# include <cstring>

class Gb
{
	public:
		enum Model
		{
			Auto,
			DMG,//Game Boy (monochrome), introduced on April 21, 1989
			CGB//Game Boy Color (color), introduced on October 21, 1998
		};

	private:
		Gb::Model 		_model;			// Gb model
		bool			_play;			// playing flag
		/*
		** CPU Speed
		** (system operating frequency)
		** DMG  CPU
		** 1.05 MHz
		** CGB  CPU
		** 1.05 MHz (normal mode)
		** 2.10 MHz (double-speed mode)
		*/
		size_t			_speed;

	public:
		Gb(void);
		virtual ~Gb(void);
		Gb(Gb const & src);
		Gb & operator=(Gb const & rhs);

		// Load a cartridge
		void			load ( std::string const& cartridgePath );

		// the the GameBoy model to use
		void			setModel ( Gb::Model const& model);

		// Controls
		void			play ( void );
		void			setSpeed ( size_t const& speed); // x1 , x2, ........

		// Infos
		bool			isLoaded ( void ) const; //Singelton to check-is the current cartridge is load
		bool			isRunning ( void ) const; //Singelton to check-is the current cartridge is run
		Gb::Model		model ( void ) const;//Getter to _Model
		std::string		gameTitle ( void ) const;

	private:
		void			run ( void );
};

std::ostream & operator<<(std::ostream & o, Gb const & i);

#endif /*GB_CLASS_HPP*/
