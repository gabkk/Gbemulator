static bool status = false;//remove this static var for singleton

#include "Gb.class.hpp"

Gb::Gb (void)
{
	std::cout << "Gb Default constructor" << std::endl;
}

Gb::Gb (Gb const & src)
{
	(void)src;
}

Gb::~Gb (void)
{
	std::cout << "Gb Default destructor" << std::endl;
}

Gb & Gb::operator=(Gb const & rhs)
{
	(void)rhs;
	return *this;
}

void Gb::load (std::string const& cartridgePath)
{
	/*
	** Test if the cartridge is correct actually i just check
	** the cartridge name and path but i think
	** we need to implement an other way to check if the 
	** cartridge is correct
	*/

    FILE* fp;
	
	fp = std::fopen(cartridgePath.c_str(), "r");
	if(!fp)
		throw std::exception();
	else
		this->isLoaded();
	if (cartridgePath.find(".gbc") != std::string::npos)
	{
		std::cout << "gbc cartridge Load" << std::endl;
		this->setModel(CGB);
	}
	else if (cartridgePath.find(".gb")  != std::string::npos)
	{
		std::cout << "gb cartridge Load" << std::endl;
		this->setModel(DMG);
	}
	else
	{
		this->setModel(Auto);
		std::cout << "Not found " << std::endl;
	}
	this->run();//faire un try catch
}

void Gb::setModel (Gb::Model const& model)
{
	this->_model = model;
}


bool Gb::isLoaded (void) const
{
	return (status = true);
}

bool Gb::isRunning (void) const
{
	return (status = true);
}

Gb::Model Gb::model (void) const
{
	return (_model);
}

std::string Gb::gameTitle (void) const
{
	/*si run throw an error return false otherwhise return true*/
	return ("loll");
}

void Gb::run (void)
{
	std::cout << "Run gb" << std::endl;
	std::cout << "Model value: " << _model << std::endl;
}

std::ostream & operator<<(std::ostream & o, Gb const & i)
{
	(void)i;
	return o;
}
