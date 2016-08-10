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

// void Cpu::loadCartridge ( std::string const& cartridgePath, Gb::Model const& model )
// {

// }


Memory	*Cpu::memory(void) const
{
	return (this->_memory);
}

Cartridge	*Cpu::cartridge(void) const
{
	return (this->_cartridge);
}
