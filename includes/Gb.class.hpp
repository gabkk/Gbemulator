#ifndef GB_CLASS_HPP
# define GB_CLASS_HPP

# include <iostream>
# include <fstream>

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

		Gb::Model		model ( void ) const;


		// the the GameBoy model to use
		void			setModel ( Gb::Model const& model);

	private:
		void			run ( void );
};

std::ostream & operator<<(std::ostream & o, Gb const & i);

#endif /*GB_CLASS_HPP*/
