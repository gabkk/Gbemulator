/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Registers.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhismans <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/20 17:00:37 by hhismans          #+#    #+#             */
/*   Updated: 2016/08/20 19:28:24 by hhismans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Registers.class.hpp"

Registers::Registers( void )
{
	_A = 0xAA;
	_B = 0xBB;
	_C = 0xCC;
	_D = 0xDD;
	_E = 0xEE;
	_F = 0xFF;
	_H = 0x11;
	_L = 0x22;
}

/*Registers::Registers const ( Registers const & src )
{
	_A = src.getA();
	_B = src.getB();
	_C = src.getC();
	_D = src.getD();
	_E = src.getE();
	_F = src.getF();
	_H = src.getH();
	_L = src.getL();
}*/

Registers::~Registers (){}

// GETTER
uint8_t Registers::getA() const {return this->_A;}
uint8_t Registers::getB() const {return _B;}
uint8_t Registers::getC() const {return _C;}
uint8_t Registers::getD() const {return _D;}
uint8_t Registers::getE() const {return _E;}
uint8_t Registers::getF() const {return _F;}
uint8_t Registers::getH() const {return _H;}
uint8_t Registers::getL() const {return _L;}

uint16_t Registers::getAF() const {return ((_A << 8) + _F);}
uint16_t Registers::getBC() const {return ((_B << 8) + _C);}
uint16_t Registers::getDE() const {return ((_D << 8) + _E);}
uint16_t Registers::getHL() const {return ((_H << 8) + _L);}

//SETTER
void Registers::setA(uint8_t value){_A = value;}
void Registers::setB(uint8_t value){_B = value;}
void Registers::setC(uint8_t value){_C = value;}
void Registers::setD(uint8_t value){_D = value;}
void Registers::setE(uint8_t value){_E = value;}
void Registers::setF(uint8_t value){_F = value;}
void Registers::setH(uint8_t value){_H = value;}
void Registers::setL(uint8_t value){_L = value;}

void Registers::setAF(uint16_t value){
	_A = (value >> 8);
	_F = (value & 0x00FF);
}
void Registers::setBC(uint16_t value){
	_B = (value >> 8);
	_C = (value & 0x00FF);
}

void Registers::setDE(uint16_t value){
	_D = (value >> 8);
	_E = (value & 0x00FF);
}

void Registers::setHL(uint16_t value){
	_H = (value >> 8);
	_L = (value & 0x00FF);
}

