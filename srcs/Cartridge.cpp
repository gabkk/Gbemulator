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
	this->_model = model;
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
	std::stringstream ss;
	unsigned int x;
	unsigned int out;

	ss << std::hex << addr;
	ss >> x;
	out = static_cast<int>(x);

	return (this->data()[out]);
}

void Gbmu::Cartridge::setByteAt ( uint16_t const& addr , uint8_t const& value )
{
	(void)addr;
	(void)value;

}

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
	return (this->_header);
}

uint8_t *Gbmu::Cartridge::data (void) const
{
	return (this->_data);
}

std::string const& Gbmu::Cartridge::path (void) const
{
	return (this->_path);
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

	delete memblock;
	std::string stringOfHex = ToHex(std::string(memblock, size_of_file), true);

	std::stringstream convertStream;
	size_t offset = 0, i = 0;
	
	/*
	** Allocate the data tab
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
	std::cout << hex(this->_header.CGB_flag) << std::endl;
	if (this->_header.CGB_flag == 0x80)
		this->setModel(Gbmu::Gb::Auto);
	else if (this->_header.CGB_flag == 0xC0)
		this->setModel(Gbmu::Gb::CGB);
	else if (this->_header.CGB_flag == 0x00)
		this->setModel(Gbmu::Gb::DMG);

/*
** new_license_code
*/
	this->_header.new_license_code = ((this->getByteAt(0x144) << 8 )+ this->getByteAt(0x145));
	std::cout << "header.new_license_code" << std::endl;
	std::cout << hex(this->_header.new_license_code) << std::endl;

/*
** SGB_flag
*/
	this->_header.SGB_flag = this->getByteAt(0x146);
	std::cout << "header.SGB_flag" << std::endl;
	std::cout << hex(this->_header.SGB_flag) << std::endl;

/*
** cartridge_type
*/
	this->_header.cartridge_type = this->getByteAt(0x147);
	std::cout << "header.cartridge_type" << std::endl;
	std::cout << hex(this->_header.cartridge_type) << std::endl;

/*
** rom_size
*/
	this->_header.rom_size = this->getByteAt(0x148);
	std::cout << "header.rom_size" << std::endl;
	std::cout << hex(this->_header.rom_size) << std::endl;

/*
** ram_size
*/
	this->_header.ram_size = this->getByteAt(0x149);
	std::cout << "header.ram_size" << std::endl;
	std::cout << hex(this->_header.ram_size) << std::endl;

/*
** destination_code
*/
	this->_header.destination_code = this->getByteAt(0x14A);
	std::cout << "header.destination_code" << std::endl;
	std::cout << hex(this->_header.destination_code) << std::endl;

/*
** old_license_code
*/
	this->_header.old_license_code = this->getByteAt(0x14B);
	std::cout << "header.old_license_code" << std::endl;
	std::cout << hex(this->_header.old_license_code) << std::endl;

/*
** rom_version
*/
	this->_header.rom_version = this->getByteAt(0x14C);
	std::cout << "header.rom_version" << std::endl;
	std::cout << hex(this->_header.rom_version) << std::endl;

/*
** header_checksum
*/
	this->_header.header_checksum = this->getByteAt(0x14D);
	std::cout << "header.header_checksum" << std::endl;
	std::cout << hex(this->_header.header_checksum) << std::endl;

/*
** global_checksum
*/
	this->_header.global_checksum = ((this->getByteAt(0x14E) << 8 )+ this->getByteAt(0x14F));
	std::cout << "header.global_checksum" << std::endl;
	std::cout << hex(this->_header.global_checksum) << std::endl;

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