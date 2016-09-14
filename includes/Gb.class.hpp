#ifndef GB_CLASS_HPP
# define GB_CLASS_HPP

/*
** This class represent the game boy object
*/

# include <iostream>
# include <fstream>
# include <string>
# include <cstring>
# include <sstream> //for osstringstream
# include <iomanip>      // std::setfill, std::setw

namespace Gbmu
{
class Cpu;
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
	Cpu*			_cpu;			// the gameboy CPU
	// Debugger*	_debugger;		// the gameboy debugger
	// std::thread*	_thread;		// running thread
	bool			_play;			// playing flag
	size_t			_speed;

public:
	Gb(void);
	virtual ~Gb(void);
	Gb(Gb const & src);
	Gb & operator=(Gb const & rhs);

	// Load a cartridge
	void			load ( std::string const& cartridgePath );
	/*NI*/	// bool			saveState ( std::string const& path );
	/*NI*/	// bool			loadState ( std::string const& path );

	// // set your gui screen to gameBoy screen
	/*NI*/	// void			setScreen ( IScreen* screen );

	// the the GameBoy model to use
	void			setModel ( Gb::Model const& model);

	// Controls
	void			play ( void );
	void			pause ( void );
	/*NI*/	//	void			reset ( void );
	void			setSpeed ( size_t const& speed); // x1 , x2, ........
	/*NI*/	//	void			mute ( bool const& b );

	// Infos
	bool			isLoaded ( void ) const; //Singelton to check-is the current cartridge is load
	bool			isRunning ( void ) const; //Singelton to check-is the current cartridge is run
	/*NI*/	// size_t			speed ( void ) const;
	Gb::Model		model ( void ) const;//Getter to _Model
	std::string		gameTitle ( void ) const;

	Cpu 			* cpu(void) const;

	// components
	/*NI*/	// IDebugger*		debugger ( void ) const;
	/*NI*/	// IKeyPad*			keyPad ( void ) const;

private:
	void			_run ( void );
};
}
#else
namespace Gbmu
{
class Gb;
}
#endif /*GB_CLASS_HPP*/
