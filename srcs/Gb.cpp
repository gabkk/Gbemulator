#include "Gb.class.hpp"

Gb::Gb (void)
{
	std::cout << "Create gb" << std::endl;
	this->run();
}

Gb::Gb (Gb const & src)
{
	(void)src;
}

Gb::~Gb (void)
{
	std::cout << "Destroy gb" << std::endl;
}

Gb & Gb::operator=(Gb const & rhs)
{
	(void)rhs;
	return *this;
}

void Gb::load (std::string const& cartridgePath)
{
	/*
	** Test if the cartridge is correct
	** Actually i just check the cartridge name but i think
	** we need to implement an other way to check if the 
	** cartridge is correct
	*/

	int cartridge_is_correct;
	
	cartridge_is_correct = 0;
	if (cartridgePath.find(".gbc") != std::string::npos)
	{
		std::cout << "gbc cartridge " << std::endl;
		this->setModel(CGB);
		cartridge_is_correct = 1;
	}
	else if (cartridgePath.find(".gb")  != std::string::npos)
	{
		std::cout << "gb cartridge " << std::endl;
		this->setModel(DMG);
		cartridge_is_correct = 1;
	}
	else
	{
		this->setModel(Auto);
		std::cout << "Not found " << std::endl;
	}
}

void Gb::setModel (Gb::Model const& model)
{
	this->_model = model;
}

bool Gb::isLoaded (void) const
{
	/*if Gb::load throw an error return false otherwhise return true*/

	return (true);
}

bool Gb::isRunning (void) const
{
	/*si run throw an error return false otherwhise return true*/
	return (true);
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
