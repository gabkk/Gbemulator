# include "../includes/Cpu.class.hpp"

Gbmu::Cpu::Cpu (void) :
   _regs(new Gbmu::Registers),
   _memory(new Gbmu::Memory(this))
{
	//call the bios if success ->
	//this->_BOOT = true;
    //Memory			memory(this);

    //this->_memory = &memory;
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
	//Initializied it with new
	Cartridge	cartridge(this, cartridgePath, model);

	this->_cartridge = &cartridge;
	
	/*
	**	i try to add the cartridge->_data to the memory
	**	but i don't have time too :)
	*/

//	this->_memory->onWriteVBK(&this->_cartridge->data());
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

Gbmu::Registers * Gbmu::Cpu::regs(void) const {
    return (_regs);
}
