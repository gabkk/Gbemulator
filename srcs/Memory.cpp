#include "Memory.class.hpp"

Gbmu::Memory::Memory (Cpu *cpu)
{
	this->_cpu = cpu;
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