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
			DMG,
			CGB
		};

	private:
		Gb::Model 		_model;			// Gb model

	public:
		Gb(void);
		virtual ~Gb(void);
		Gb(Gb const & src);
		Gb & operator=(Gb const & rhs);

		// Load a cartridge
		void			load ( std::string const& cartridgePath );

		// the the GameBoy model to use
		void			setModel ( Gb::Model const& model);

		// Infos
		bool			isLoaded ( void ) const;
		bool			isRunning ( void ) const;
		Gb::Model		model ( void ) const;
		std::string		gameTitle ( void ) const;

	private:
		void			run ( void );
};

std::ostream & operator<<(std::ostream & o, Gb const & i);

#endif /*GB_CLASS_HPP*/
