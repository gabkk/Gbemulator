# include "Cpu.class.hpp"

Gbmu::Cpu::Cpu (void)
{

}

Gbmu::Cpu::Cpu (Cpu const & src)
{
	(void)src;
}

Gbmu::Cpu::~Cpu (void)
{

}

Gbmu::Cpu & Gbmu::Cpu::operator=(Cpu const & rhs)
{
	(void)rhs;
	return *this;
}

// void Gbmu::Cpu::loadCartridge ( std::string const& cartridgePath, Gb::Model const& model )
// {

// }


Gbmu::Memory	*Gbmu::Cpu::memory(void) const
{
	return (this->_memory);
}

Gbmu::Cartridge	*Gbmu::Cpu::cartridge(void) const
{
	return (this->_cartridge);
}
