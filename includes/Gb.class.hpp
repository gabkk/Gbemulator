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

	public:
		Gb(void);
		~Gb(void);
		Gb(Gb const & src);
		Gb & operator=(Gb const & rhs);
		void			run ( void );

	private:
};

std::ostream & operator<<(std::ostream & o, Gb const & i);

#endif /*GB_CLASS_HPP*/
