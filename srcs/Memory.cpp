#include "Memory.class.hpp"

Memory::Memory (Cpu *cpu)
{
	this->_cpu = cpu;
}

Memory::Memory (Memory const & src)
{
	(void)src;
}

Memory::~Memory (void)
{
}

Memory & Memory::operator=(Memory const & rhs)
{
	(void)rhs;
	return *this;
}