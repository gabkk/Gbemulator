#include "Gb.class.hpp"

Gb::Gb(void)
{
	std::cout << "Create gb" << std::endl;
}

Gb::Gb(Gb const & src)
{
	(void)src;
}

Gb::~Gb(void)
{
	std::cout << "Destroy gb" << std::endl;
}

Gb & Gb::operator=(Gb const & rhs)
{
	(void)rhs;
	return *this;
}

void Gb::run(void)
{
	std::cout << "Run gb" << std::endl;
}

std::ostream & operator<<(std::ostream & o, Gb const & i)
{
	(void)i;
	return o;
}
