# include "Cartridge.class.hpp"

Gbmu::Cartridge::Cartridge (Gbmu::Cpu* cpu, std::string const& path , Gb::Model const& model)
{
	this->_cpu = cpu;
	this->_path = path;
	this->load();	
	(void)model;
}

Gbmu::Cartridge::Cartridge (Cartridge const & src)
{
	(void)src;
}

Gbmu::Cartridge::~Cartridge (void)
{

}

Gbmu::Cartridge & Gbmu::Cartridge::operator=(Cartridge const & rhs)
{
	(void)rhs;
	return *this;
}

void Gbmu::Cartridge::reset (void)
{

}

void Gbmu::Cartridge::setModel (Gb::Model const& model )
{
	(void)model;

}

std::string Gbmu::Cartridge::title (void) const
{
	return (this->_header.title);
}

bool Gbmu::Cartridge::colorGB (void) const
{
	return (false);
}

bool Gbmu::Cartridge::colorCart (void) const
{
	return (false);
}

uint8_t const& Gbmu::Cartridge::getByteAt ( uint16_t const& addr )
{
	uint8_t *byte;
	std::stringstream ss;
	unsigned int x;
	unsigned int out;

	byte = new uint8_t;
	ss << std::hex << addr;
	ss >> x;
	out = static_cast<int>(x);
	*byte = this->data()[out];

	return (*byte);
}

void Gbmu::Cartridge::setByteAt ( uint16_t const& addr , uint8_t const& value )
{
	(void)addr;
	(void)value;

}

int Get_pos(uint16_t const& addr)
{

    std::stringstream ss;
    unsigned int x;
    unsigned int out;

    ss << std::hex << addr;
    ss >> x;
	out = static_cast<int>(x)*2;

	return out;
}

// uint8_t Get_pos_in_tab(uint16_t const& addr)
// {

//     std::stringstream ss;
//     unsigned int x;
//     unsigned int out;

//     ss << std::hex << addr;
//     ss >> x;
// 	out = static_cast<int>(x);

// 	return (this->data()[out]);
// }

char HexToCharpos(int addr)
{

    std::stringstream ss;
    unsigned int x;

    ss << std::hex << addr;
    ss >> x;

	return x;
}

struct Gbmu::Cartridge::Header const& Gbmu::Cartridge::header (void) const
{
/*
**	Header Temporaire 
*/

	struct Gbmu::Cartridge::Header *header;
	header = new struct Gbmu::Cartridge::Header;

	return (*header);
}

uint8_t *Gbmu::Cartridge::data (void) const
{
	return (this->_data);
}

std::string const& Gbmu::Cartridge::path (void) const
{
	std::string const *path;

	path = new std::string;
	path = &this->_path;
	return (*path);
}

void Gbmu::Cartridge::saveState ( std::fstream& file)
{
	(void)file;

}

void Gbmu::Cartridge::loadState ( std::fstream& file)
{
	(void)file;
}

/*
**	Convert a binary file to a string fill a hex
**
*/

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

/*
** Trick to show hex on std::cout
**
*/


struct HexCharStruct
{
  unsigned char c;
  HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
  return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(uint8_t _c)
{
  return HexCharStruct(_c);
}

/*
**	This methode open the file associate to the path
**	test if this path is correct and extract the binary
**	code. This code is converted to an Hex value and
**	store to the uint8_t* _data private variable
**
*/

void						Gbmu::Cartridge::load ( void )
{
	std::ifstream::pos_type size_of_file;
	Gb						gb;
	char 					*memblock = NULL;
	
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
		size_of_file = file.tellg();
		memblock = new char [size_of_file];
		file.seekg (0, std::ios::beg);
		file.read (memblock, size_of_file);
		file.close();
	}
	else
		std::perror("File opening failed");

	std::string stringOfHex = ToHex(std::string(memblock, size_of_file), true);

	std::stringstream convertStream;
	size_t offset = 0, i = 0;
	
	/*
	** Allocate the tab
	*/
	this->_data = new uint8_t[stringOfHex.length()/2];

	while (offset < (stringOfHex.length()/2)) 
	{
		unsigned int buffer;

		convertStream << std::hex << stringOfHex.substr(offset, 2);         
		convertStream >> std::hex >> buffer;

		this->_data[i] = static_cast<uint8_t>(buffer);
		
//		std::cout << this->_data[i] << std::endl;

		offset += 2;
		i++;

		// empty the stringstream
		convertStream.str(std::string());
		convertStream.clear();
	}

	this->_header = this->header();

	int position = 0;
	int length;

/*
** Entry point
*/
	length = 0;
	position = 0x0100;
	while (length < 0x4)
	{
		this->_header.entry_point[length] = this->getByteAt(position + length);
		length++;
	}
	this->_header.entry_point[length] = '\0';
	std::cout << "header.entry_point" << std::endl;
	
	int j;

	j=0;
	while (j < 0x4)
	{
		std::cout << hex(this->_header.entry_point[j]);
		j++;
	}
	std::cout << std::endl;

/*
** Nintendo_logo
*/
	length = 0;
	position = 0x104;
	while (length < 0x30)
	{
		this->_header.nintendo_logo[length] = this->getByteAt(position + length);
		length++;
	}
	this->_header.nintendo_logo[length] = '\0';
	std::cout << "header.nintendo_logo" << std::endl;

	j=0;
	while (j < 0x30)
	{
		std::cout << hex(this->_header.nintendo_logo[j]);
		j++;
	}
	std::cout << std::endl;

/*
** Title
*/
	length = 0;
	position = 0x134;
	while (length < 0x0F)
	{
		this->_header.title[length] = HexToCharpos(this->getByteAt(position + length));
		length++;
	}
	this->_header.title[length] = '\0';
	std::cout << "header.title" << std::endl;
	std::cout << this->_header.title << std::endl;

/*
** CGB_flag
*/
	this->_header.CGB_flag = this->getByteAt(0x143);
	std::cout << "header.CGB_flag" << std::endl;
	std::cout << hex(this->_header.CGB_flag);

/*
** new_license_code
*/
	// this->_header.new_license_code = 
	// this->getByteAt(0x144) << 8 + this->getByteAt(0x145);
	// std::cout << "header.new_license_code" << std::endl;
	// std::cout << hex(this->_header.new_license_code);

	// std::cout << "Infos recuper dans Gbmu::Cartridge::load && Gbmu::Cartridge::header" << std::endl;

	/*Test if the header title value is set correctly*/
	/*
	** std::cout << this->title() <<std::endl;
	*/

	/*Display data available inside this->data*/
	/*
	** std::cout << this->data() <<std::endl;
	*/
}