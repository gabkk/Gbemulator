# include "../includes/Cpu.class.hpp"

Gbmu::Cpu::Cpu (void) :
	_regs(new Gbmu::Registers),						// initialize registers
	_memory(new Gbmu::Memory),						// initialize memory
	_cartridge(NULL),								// no cartridge is initially loaded
	_instructions(new Gbmu::Instructions(this)),	// cpu instruction set
	_BOOT(true),									// start the gameboy
	_HALT(false)									// don't halt
{}

Gbmu::Cpu::~Cpu (void) {}

void Gbmu::Cpu::loadCartridge ( std::string const& cartridgePath, Gb::Model const& model )
{
	uint8_t		*data;

	if (_cartridge)			// if a cartridge was already loaded
		delete _cartridge;	// delete it
	_cartridge = new Gbmu::Cartridge(cartridgePath, model);		// create new Cartridge object
	data = _cartridge->data();
	for (int addr = 0; addr < CARTRIDGE_SIZE; addr++) {
		_memory->setByteAt(addr, data[addr]);
	}
}

void Gbmu::Cpu::executeFrame(void) {
	uint8_t		instruction;

	std::cout << "executeFrame at " << std::hex << _pc << std::endl;
	instruction = _memory->getByteAt(_pc);
	std::cout << " instruction = " << std::hex << static_cast<uint16_t>(instruction) << std::endl;
	_instructions->execute(instruction);
}

void Gbmu::Cpu::setHALT ( bool const& b ) { _HALT = b; }

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

Gbmu::Memory		*Gbmu::Cpu::memory(void) const { return (this->_memory); }

Gbmu::Cartridge		*Gbmu::Cpu::cartridge(void) const { return (this->_cartridge); }

Gbmu::Registers		*Gbmu::Cpu::regs(void) const { return (_regs); }

