# include "../includes/Cpu.class.hpp"

Gbmu::Cpu::Cpu (void) :
	_regs(new Gbmu::Registers),	// initialize registers
	_memory(new Gbmu::Memory),	// initialize memory
	_cartridge(NULL),			// no cartridge is initially loaded
	_pc(DEFAULT_PC),			// program entry point is at 0x150
	_sp(DEFAULT_SP),			// stack pointer default is 0xFFFE
	_BOOT(true),
	_HALT(false)
{}

Gbmu::Cpu::~Cpu (void) {}

void Gbmu::Cpu::loadCartridge ( std::string const& cartridgePath, Gb::Model const& model )
{
	uint8_t		*data;

	if (_cartridge)
		delete _cartridge;
	_cartridge = new Gbmu::Cartridge(cartridgePath, model);
	data = _cartridge->data();
	for (int addr = 0; addr < CARTRIDGE_SIZE; addr++) {
		_memory->setByteAt(addr, data[addr]);
	}
}

void Gbmu::Cpu::executeFrame(void) {
	uint8_t		instruction;

	std::cout << "executeFrame at " << std::hex << _pc << std::endl;
	instruction = _memory->getByteAt(_pc);
	std::cout << "next instruction = " << std::hex << static_cast<uint16_t>(instruction) << std::endl;
	_pc += 1;
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
