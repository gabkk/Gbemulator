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
	return (this->_header.title);
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
	uint8_t *byte;

/*Work in progresse*/


/* Format addr correctyle*/

    // unsigned int x;   
    // std::stringstream ss;
    // ss << std::hex << "fffefffe";
    // ss >> x;
	// static_cast<int>(x)
/*tmp*/
    
    std::stringstream ss;
    unsigned int x;
	byte = new uint8_t;

    ss << std::hex << addr;
    ss >> x;

	*byte = static_cast<int>(x);

	std::cout << x << std::endl;
//	std::cout << *byte << std::endl;
	(void)addr;
	return (*byte);
}

void Cartridge::setByteAt ( uint16_t const& addr , uint8_t const& value )
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

char HexToCharpos(uint8_t * addr)
{

    std::stringstream ss;
    unsigned int x;

    ss << std::hex << addr;
    ss >> x;

	return x;
}

struct Cartridge::Header const& Cartridge::header (void) const
{
/*
**	Header Temporaire 
*/

	struct Cartridge::Header *header;
	header = new struct Cartridge::Header;

	int position; 
	int length;

/*
** Entry point
*/
	length = 0;
	position = Get_pos(0x100);
	std::cout << "Adresse of header->entry_point[length] : " << &header->entry_point << std::endl;
	while (length < 8)
	{
		header->entry_point[length] = (this->data())[position + length];
		length++;
	}
	header->entry_point[length] = '\0';
	std::cout << "header.entry_point" << std::endl;
	std::cout << header->entry_point << std::endl;

/*
** Nintendo_logo
*/
	length = 0;
	position = Get_pos(0x104);
	std::cout << "Adresse of header->nintendo_logo : " << &header->nintendo_logo << std::endl;
	while (length < 96)
	{
		header->nintendo_logo[length] = ((this->data())[position + length]);
		length++;
	}
	header->nintendo_logo[length] = '\0';
	std::cout << "header.nintendo_logo" << std::endl;
	std::cout << header->nintendo_logo << std::endl;

/*
** Title
*/
	length = 0;
	position = Get_pos(0x134);
	uint8_t *tmp2;
	tmp2 = new uint8_t[2];
	int cmpt = 0;
	std::cout << "Adresse of header->title : " << &header->title << std::endl;
	while (length < 30)
	{
		tmp2[0] = ((this->data())[position + length]);
		tmp2[1] = ((this->data())[position + length + 1]);
		tmp2[2] = '\0';
	header->title[cmpt] = HexToCharpos(tmp2);
		length+=2;
		cmpt++;
	}
	header->title[cmpt] = '\0';
	delete tmp2;
	std::cout << "header->title : " << header->title << std::endl;

	return (*header);
}

uint8_t *Cartridge::data (void) const
{
	return (this->_data);
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
**	This methode open the file associate to the path
**	test if this path is correct and extract the binary
**	code. This code is converted to an Hex value and
**	store to the uint8_t* _data private variable
**
*/

void						Cartridge::load ( void )
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

	std::string tohexed = ToHex(std::string(memblock, size_of_file), true);
	this->_data = reinterpret_cast<uint8_t*>(&tohexed[0]);
	this->_header = this->header();

	std::cout << std::endl;
	std::cout << std::endl;
	/*Test if the header title value is set correctly*/
	std::cout << this->title() <<std::endl;
}