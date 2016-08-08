# include "Cartridge.class.hpp"

Cartridge::Cartridge (/*Cpu* cpu,*/ std::string const& path , Gb::Model const& model)
{
	this->_path = path;
	this->load();
	(void)model;
}

Cartridge::Cartridge (Cartridge const & src)
{
	(void)src;
}

Cartridge::~Cartridge (void)
{

}

Cartridge & Cartridge::operator=(Cartridge const & rhs)
{
	(void)rhs;
	return *this;
}

void Cartridge::reset (void)
{

}

void Cartridge::setModel (Gb::Model const& model )
{
	(void)model;

}

std::string Cartridge::title (void) const
{
	return ("title");
}

bool Cartridge::colorGB (void) const
{
	return (false);
}

bool Cartridge::colorCart (void) const
{
	return (false);
}

uint8_t const& Cartridge::getByteAt ( uint16_t const& addr )
{
	uint8_t const *byte;

	(void)addr;
	byte = new uint8_t;
	return (*byte);
}

void Cartridge::setByteAt ( uint16_t const& addr , uint8_t const& value )
{
	(void)addr;
	(void)value;

}

struct Cartridge::Header const& Cartridge::header (void) const
{
	struct Cartridge::Header *test;

	test = new struct Cartridge::Header;
	return (*test);
}

uint8_t *Cartridge::data (void) const
{
	return (0);
}

std::string const& Cartridge::path (void) const
{
	std::string const *path;

	path = new std::string;
	path = &this->_path;
	return (*path);
}

void Cartridge::saveState ( std::fstream& file)
{
	(void)file;

}

void Cartridge::loadState ( std::fstream& file)
{
	(void)file;
}

void Cartridge::load ( void )
{
	Gb			gb;
	
	try
	{
		gb.load(this->path());
	}
	catch (std::exception& e)
	{
		std::perror("File opening failed");
	}
}