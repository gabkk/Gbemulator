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


std::string ToHex(const std::string& s, bool upper_case)
{
	std::ostringstream ret;

	for (std::string::size_type i = 0; i < s.length(); ++i)
	{
		int z = s[i]&0xff;
		ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << z;
	}

	return ret.str();
}

void						Cartridge::load ( void )
{
	std::ifstream::pos_type size;
	Gb						gb;
	char 					*memblock;
	
	try
	{
		gb.load(this->path());
	}
	catch (std::exception& e)
	{
		std::perror("File opening failed");
	}

	std::ifstream file(this->path().c_str(), std::ios::in|std::ios::binary|std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, std::ios::beg);
		file.read (memblock, size);
		file.close();

		std::cout << "the complete file content is in memory" << std::endl;

		std::string tohexed = ToHex(std::string(memblock, size), true);


		std::cout << tohexed << std::endl;
		
	}
	else
		throw std::exception();
}