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
	if (this->_model == Gb::CGB)
		return (true);
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

char HexToCharpos(int addr)
{

	std::stringstream ss;
	unsigned int x;

	ss << std::hex << addr;
	ss >> x;
	return x;
}

long getFileSize(FILE *file)
{
	long lCurPos, lEndPos;
	lCurPos = ftell(file);
	fseek(file, 0, 2);
	lEndPos = ftell(file);
	fseek(file, lCurPos, 0);
	return lEndPos;
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
	const char *filePath;
	FILE *file;
	long fileSize;

	filePath = this->path().c_str();
	// Open the file in binary mode using the "rb" format string
	// This also checks if the file exists and/or can be opened for reading correctly
	if ((file = fopen(filePath, "rb")) == NULL)
		std::cout << "Could not open specified file" << std::endl;
	else
		std::cout << "File opened successfully" << std::endl;

	// Get the size of the file in bytes
	fileSize = getFileSize(file);

	// Allocate space in the buffer for the whole file
	this->_data = new uint8_t[fileSize];

	// Read the file in to the buffer
	fread(this->_data, fileSize, 1, file);
	fclose(file);


	/*
	**	Store data from this_data to Header
	**	i use getBytAt whitch takes an adresse
	**	and return the uint8 value whtich match with the index
	**
	**
	**	All the std::cout are there to display debug we can remove
	**	i leave it to let you see the value on the output
	*/

	int position = 0;
	int length;
	int j;

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
	j=0;
	while (j < 0x4)
	{
		printf("%X ", this->_header.entry_point[j]);
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
		printf("%X ", this->_header.nintendo_logo[j]);
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

	printf("%X \n", this->_header.CGB_flag);

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
	printf("%X \n", this->_header.new_license_code);

	/*
	** SGB_flag
	*/
	this->_header.SGB_flag = this->getByteAt(0x146);
	std::cout << "header.SGB_flag" << std::endl;
	printf("%X \n", this->_header.SGB_flag);

	/*
	** cartridge_type
	*/
	this->_header.cartridge_type = this->getByteAt(0x147);
	std::cout << "header.cartridge_type" << std::endl;
	printf("%X \n", this->_header.cartridge_type);

	/*
	** rom_size
	*/
	this->_header.rom_size = this->getByteAt(0x148);
	std::cout << "header.rom_size" << std::endl;
	printf("%X \n", this->_header.rom_size);

	/*
	** ram_size
	*/
	this->_header.ram_size = this->getByteAt(0x149);
	std::cout << "header.ram_size" << std::endl;
	printf("%X \n", this->_header.ram_size);

	/*
	** destination_code
	*/
	this->_header.destination_code = this->getByteAt(0x14A);
	std::cout << "header.destination_code" << std::endl;
	printf("%X \n", this->_header.destination_code);

	/*
	** old_license_code
	*/
	this->_header.old_license_code = this->getByteAt(0x14B);
	std::cout << "header.old_license_code" << std::endl;
	printf("%X \n", this->_header.old_license_code);

	/*
	** rom_version
	*/
	this->_header.rom_version = this->getByteAt(0x14C);
	std::cout << "header.rom_version" << std::endl;
	printf("%X \n", this->_header.rom_version);

	/*
	** header_checksum
	*/
	this->_header.header_checksum = this->getByteAt(0x14D);
	std::cout << "header.header_checksum" << std::endl;
	printf("%X \n", this->_header.header_checksum);

	/*
	** global_checksum
	*/
	this->_header.global_checksum = ((this->getByteAt(0x14E) << 8 )+ this->getByteAt(0x14F));
	std::cout << "header.global_checksum" << std::endl;
	printf("%X \n", this->_header.global_checksum);

	// Test
	std::cout << "0x100: " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)this->getByteAt(0x100)  << "." << std::endl;
	std::cout << "0x101: " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)this->getByteAt(0x101)  << "." << std::endl;
	std::cout << "0x102: " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)this->getByteAt(0x102)  << "." << std::endl;
	std::cout << "0x103: " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)this->getByteAt(0x103)  << "." << std::endl;
}
