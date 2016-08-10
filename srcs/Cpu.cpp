# include "Cpu.class.hpp"

Cpu::Cpu (void)
{

}

Cpu::Cpu (Cpu const & src)
{
	(void)src;
}

Cpu::~Cpu (void)
{

}

Cpu & Cpu::operator=(Cpu const & rhs)
{
	(void)rhs;
	return *this;
}

Memory	*Memory(void) const
{

}

Cartridge	*cartridge(void) const
{

}
