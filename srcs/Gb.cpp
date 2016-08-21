# include "../includes/Gb.class.hpp"
# include "../includes/Cpu.class.hpp"

Gbmu::Gb::Gb (void)
{
	Gbmu::Cpu			*cpu;

	cpu = new Gbmu::Cpu;
	this->_cpu = cpu;
	this->setModel(Auto);
	this->_cpu->setHALT(false);
	this->_run();
}

Gbmu::Gb::Gb (Gb const & src)
{
	(void)src;
}

Gbmu::Gb::~Gb (void)
{
	delete this->_cpu;
}

Gbmu::Gb & Gbmu::Gb::operator=(Gb const & rhs)
{
	(void)rhs;
	return *this;
}

/*
** Load a cartridge
*/


void Gbmu::Gb::load (std::string const& cartridgePath)
{
	this->_cpu->loadCartridge(cartridgePath, Auto);
	if (this->_cpu->onBoot() == true &&
		this->_cpu->onHalt() == false)
	{
		this->_play = true;
	}
	else
		std::cout << "Boot status: Failed" << std::endl; // Check if it's correct
//	this->isLoaded();
}

/* 
** The GameBoy model to use
*/

void Gbmu::Gb::setModel (Gbmu::Gb::Model const& model)
{
	this->_model = model;
}

/*
** Controls
*/

void Gbmu::Gb::play (void)
{
	//If Interact with Gui interface button
//	if 
	this->_play = true;

	//Cpu launch executeframe
}

void Gbmu::Gb::pause (void)
{
	//If Interact with Gui interface button
	this->_play = false;

	//Cpu stop executeframe

}

void Gbmu::Gb::setSpeed(size_t const& speed)
{
	(void)speed; // TODO
}

/*
** Infos
*/

bool Gbmu::Gb::isLoaded (void) const
{
	return (false);
}

bool Gbmu::Gb::isRunning (void) const
{
	return (false);
}

Gbmu::Gb::Model Gbmu::Gb::model (void) const
{
	return (this->_model);
}

std::string Gbmu::Gb::gameTitle (void) const
{
	return ("indefined");
}

Gbmu::Cpu * Gbmu::Gb::cpu(void) const {
    return _cpu;
}

/*
** Private
*/

void Gbmu::Gb::_run (void)
{
//	bool value = false;//Get the Gui button play, if play so HALT false

	if (this->model() == DMG)
		this->_speed = 1;
	else if (this->model() == CGB)
		this->_speed = 1;
	this->play();
}
