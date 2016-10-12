#include "../includes/Memory.class.hpp"

Gbmu::Memory::Memory (void) :
	_data(new uint8_t[GB_MEM_SIZE])
{
}

Gbmu::Memory::Memory (Memory const & src)
{
	(void)src;
}

Gbmu::Memory::~Memory (void)
{
	delete[] _data;
}

Gbmu::Memory & Gbmu::Memory::operator=(Memory const & rhs)
{
	(void)rhs;
	return *this;
}

void Gbmu::Memory::onWriteVBK( uint8_t const& value )
{
	(void)value;
	// this->_data = value;
}


/**
  * Read one byte at address and return it
  * @param addr Address at which we read a byte
  * @return The byte at addr in gb memory
 */
uint8_t Gbmu::Memory::getByteAt(uint16_t const& addr) const {
	return _data[addr];
}

/**
  * Set byte at address
  * @param addr Address at which we set a byte (value)
  * @param value Value we set at addr
 */
void Gbmu::Memory::setByteAt(const uint16_t &addr, const uint8_t &value) {
	_data[addr] = value;
}
