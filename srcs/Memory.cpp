#include "../includes/Memory.class.hpp"

Gbmu::Memory::Memory (Cpu *cpu) :
    _cpu(cpu)
{
	_data = new uint8_t[GB_MEM_SIZE];
}

Gbmu::Memory::Memory (Memory const & src)
{
	(void)src;
}

Gbmu::Memory::~Memory (void)
{
	delete [] _data;
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
