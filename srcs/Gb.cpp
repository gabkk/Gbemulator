#include "Gb.class.hpp"

Gb::Gb(void)
{
	std::cout << "Create gb" << std::endl;
	this->run();
}

Gb::Gb(Gb const & src)
{
	(void)src;
}

Gb::~Gb(void)
{
	std::cout << "Destroy gb" << std::endl;
}

Gb & Gb::operator=(Gb const & rhs)
{
	(void)rhs;
	return *this;
}

Gb::Model Gb::model(void) const
{
	return (_model);
}

void Gb::run(void)
{
	std::cout << "Run gb" << std::endl;
	this->setModel(Auto);
	std::cout << "Model value: " << _model << std::endl;
}

void Gb::setModel(Gb::Model const& model)
{
	this->_model = model;
}

std::ostream & operator<<(std::ostream & o, Gb const & i)
{
	(void)i;
	return o;
}
