static bool *status_load = 0;
static bool *status_running = 0;

#include "Gb.class.hpp"

Gb::Gb (void) : _model(Auto), _play(false)
{

}

Gb::Gb (Gb const & src)
{
	(void)src;
}

Gb::~Gb (void)
{
	delete status_load;
	delete status_running;
}

Gb & Gb::operator=(Gb const & rhs)
{
	(void)rhs;
	return *this;
}

/*
** Load a cartridge
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

void Gb::load (std::string const& cartridgePath)
{
	/*
	** Test if the cartridge is correct actually i just check
	** the cartridge name and path but i think
	** we need to implement an other way to check if the 
	** cartridge is correct
	*/

	std::ifstream::pos_type size;
	char *memblock;

	std::ifstream file(cartridgePath.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
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
		
		this->isLoaded();
	}
	else
		throw std::exception();
	
//	fp = std::fopen(cartridgePath.c_str(), "r");

	if (cartridgePath.find(".gbc") != std::string::npos)
	{
		std::cout << "Cartridge Model CGB" << std::endl;
		this->setModel(CGB);
	}
	else if (cartridgePath.find(".gb")  != std::string::npos)
	{
		std::cout << "Cartridge Model DMG" << std::endl;
		this->setModel(DMG);
	}
	else
	{
		this->setModel(Auto);
		std::cout << "Not found " << std::endl;
	}
	this->_run();//faire un try catch


	
	// char data[65000];
	// infile >> data;
	// std::cout << "Voila les data " << std::endl << data << std::endl;
}

/* 
** The GameBoy model to use
*/

void Gb::setModel (Gb::Model const& model)
{
	this->_model = model;
}

/*
** Controls
*/

void Gb::play (void)
{
	this->_play = true;

}

void Gb::setSpeed(size_t const& speed)
{
	(void)speed; // TODO
}

/*
** Infos
*/

bool Gb::isLoaded (void) const
{
	if ((status_load = new bool) == NULL)
		return (false);
	*status_load = true;
	return (true);
}

bool Gb::isRunning (void) const
{
	if ((status_running = new bool) == NULL)
		return (false);
	*status_running = true;
	return (true);
}

Gb::Model Gb::model (void) const
{
	return (_model);
}

std::string Gb::gameTitle (void) const
{
	return ("loll");
}

/*
** Private
*/

void Gb::_run (void)
{
	std::cout << "Run gb" << std::endl;
	if (this->model() == DMG)
		this->_speed = 1;
	else if (this->model() == CGB)
		this->_speed = 2;
	this->isRunning();
	if (*status_running == true)
		this->play();
}

std::ostream & operator<<(std::ostream & o, Gb const & i)
{
	(void)i;
	return o;
}
