# include "Cpu.class.hpp"

Gbmu::Cpu::Cpu (void)
{
	//call the bios if success ->
	//this->_BOOT = true;
	this->_BOOT = true;
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

void Gbmu::Cpu::loadCartridge ( std::string const& cartridgePath, Gb::Model const& model )
{

	(void)cartridgePath;
	(void)model;
}

void Gbmu::Cpu::setHALT ( bool const& b )
{
	(void)b;
	if (b == true)
		this->_HALT = true;
	else
		this->_HALT = false;
}

void Gbmu::Cpu::stopBOOT ( void )
{
//		this->_BOOT = false;
}

bool const& Gbmu::Cpu::onHalt ( void ) const
{
	return (this->_HALT);
}

bool const& Gbmu::Cpu::onBoot ( void ) const
{
	return (this->_BOOT);
}

Gbmu::Memory	*Gbmu::Cpu::memory(void) const
{
	return (this->_memory);
}

Gbmu::Cartridge	*Gbmu::Cpu::cartridge(void) const
{
	return (this->_cartridge);
}
