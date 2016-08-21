#include "../includes/Memory.class.hpp"

Gbmu::Memory::Memory (Cpu *cpu) :
    _cpu(cpu)
{
}

Gbmu::Memory::Memory (Memory const & src)
{
	(void)src;
}

Gbmu::Memory::~Memory (void)
{
}

Gbmu::Memory & Gbmu::Memory::operator=(Memory const & rhs)
{
	(void)rhs;
	return *this;
}

void Gbmu::Memory::onWriteVBK( uint8_t const& value )
{
	(void)value;
//	this->_data = value;
}
