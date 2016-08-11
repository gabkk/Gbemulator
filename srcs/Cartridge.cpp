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

int Get_pos_in_tab(uint16_t const& addr)
{

    std::stringstream ss;
    unsigned int x;
    unsigned int out;

    ss << std::hex << addr;
    ss >> x;
	out = static_cast<int>(x);

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

struct Gbmu::Cartridge::Header const& Gbmu::Cartridge::header (void) const
{
/*
**	Header Temporaire 
*/

	struct Gbmu::Cartridge::Header *header;
	header = new struct Gbmu::Cartridge::Header;

	int position; 
	int length;

/*
** Entry point
*/
	length = 0;
	position = Get_pos(0x100);
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
	std::cout << "header.title" << std::endl;
	std::cout << header->title << std::endl;

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



	if ( this->_data[Get_pos_in_tab(0x104)] == 0xce
		&& this->_data[Get_pos_in_tab(0x105)] == 0xed //260 Car 520char donc 
		&& this->_data[Get_pos_in_tab(0x106)] == 0x66 //260 Car 520char donc 
		&& this->_data[Get_pos_in_tab(0x107)] == 0x66) //260 Car 520char donc 
		std::cout << "Works" << '\n';
	else // the 0c value
		std::cout << "No" << '\n';


		// std::cout << "Infos recuper dans Gbmu::Cartridge::load && Gbmu::Cartridge::header" << std::endl;

//	this->_header = this->header();

	/*Test if the header title value is set correctly*/
	/*
	** std::cout << this->title() <<std::endl;
	*/

	/*Display data available inside this->data*/
	/*
	** std::cout << this->data() <<std::endl;
	*/
}