/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Registers.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhismans <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/20 17:00:37 by hhismans          #+#    #+#             */
/*   Updated: 2016/10/12 01:24:00 by clement          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Registers.class.hpp"
#include <bitset>

Gbmu::Registers::Registers( void ) :
	_A(0),
	_B(0),
	_C(0),
	_D(0),
	_E(0),
	_F(0),
	_H(0),
	_L(0),
	_pc(DEFAULT_PC),	// program entry point is at 0x150
	_sp(DEFAULT_SP)	// stack pointer is initialized at 0xFFFE
{
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

Gbmu::Registers::~Registers (){}

// GETTER
uint8_t Gbmu::Registers::getA() const {return _A;}
uint8_t Gbmu::Registers::getB() const {return _B;}
uint8_t Gbmu::Registers::getC() const {return _C;}
uint8_t Gbmu::Registers::getD() const {return _D;}
uint8_t Gbmu::Registers::getE() const {return _E;}
uint8_t Gbmu::Registers::getF() const {return _F;}
uint8_t Gbmu::Registers::getH() const {return _H;}
uint8_t Gbmu::Registers::getL() const {return _L;}

uint16_t Gbmu::Registers::getAF() const {return ((_A << 8) + _F);}
uint16_t Gbmu::Registers::getBC() const {return ((_B << 8) + _C);}
uint16_t Gbmu::Registers::getDE() const {return ((_D << 8) + _E);}
uint16_t Gbmu::Registers::getHL() const {return ((_H << 8) + _L);}

uint16_t Gbmu::Registers::getPC() const { return (_pc); }
uint16_t Gbmu::Registers::getSP() const { return (_sp); }

bool Gbmu::Registers::getFz() const {return (_F >> 7);}
bool Gbmu::Registers::getFn() const {return ((_F >> 6) & 1);}
bool Gbmu::Registers::getFh() const {return ((_F >> 5) & 1);}
bool Gbmu::Registers::getFc() const {return ((_F >> 4) & 1);}

//SETTER
void Gbmu::Registers::setA(uint8_t value){_A = value;}
void Gbmu::Registers::setB(uint8_t value){_B = value;}
void Gbmu::Registers::setC(uint8_t value){_C = value;}
void Gbmu::Registers::setD(uint8_t value){_D = value;}
void Gbmu::Registers::setE(uint8_t value){_E = value;}
void Gbmu::Registers::setF(uint8_t value){_F = value;}
void Gbmu::Registers::setH(uint8_t value){_H = value;}
void Gbmu::Registers::setL(uint8_t value){_L = value;}

void Gbmu::Registers::setAF(uint16_t value){
	_A = (value >> 8);
	_F = (value & 0x00FF);
}
void Gbmu::Registers::setBC(uint16_t value){
	_B = (value >> 8);
	_C = (value & 0x00FF);
}

void Gbmu::Registers::setDE(uint16_t value){
	_D = (value >> 8);
	_E = (value & 0x00FF);
}

void Gbmu::Registers::setHL(uint16_t value){
	_H = (value >> 8);
	_L = (value & 0x00FF);
}

void Gbmu::Registers::setPC(uint16_t addr) {
	_pc = addr;
}

void Gbmu::Registers::setSP(uint16_t addr) {
	_sp = addr;
}

void Gbmu::Registers::setFz(bool value){_F |= (value << 7);}
void Gbmu::Registers::setFn(bool value){_F |= (value << 6);}
void Gbmu::Registers::setFh(bool value){_F |= (value << 5);}
void Gbmu::Registers::setFc(bool value){_F |= (value << 4);}

std::ostream &operator<<(std::ostream &out, const Gbmu::Registers &reg) {
	return out	<< "regA  = 0x" << std::hex << unsigned(reg.getA()) << std::endl
				<< "regB  = 0x" << std::hex << unsigned(reg.getB()) <<std::endl
				<< "regC  = 0x" << std::hex << unsigned(reg.getC()) <<std::endl
				<< "regD  = 0x" << std::hex << unsigned(reg.getD()) <<std::endl
				<< "regE  = 0x" << std::hex << unsigned(reg.getE()) <<std::endl
				<< "regF  = 0x" << std::hex << unsigned(reg.getF()) <<std::endl
				<< "regH  = 0x" << std::hex << unsigned(reg.getH()) <<std::endl
				<< "regL  = 0x" << std::hex << unsigned(reg.getL()) <<std::endl
				<< "regAF = 0x" << std::hex << unsigned(reg.getAF()) <<std::endl
				<< "regBC = 0x" << std::hex << unsigned(reg.getBC()) <<std::endl
				<< "regDE = 0x" << std::hex << unsigned(reg.getDE()) <<std::endl
				<< "regHL = 0x" << std::hex << unsigned(reg.getHL()) <<std::endl
				<< "regFz = 0x" << reg.getFz() <<std::endl
				<< "regFn = 0x" << reg.getFn() <<std::endl
				<< "regFh = 0x" << reg.getFh() <<std::endl
				<< "regFc = 0x" << reg.getFc() <<std::endl
				<< "regF  = Ux" << std::bitset<8>(unsigned(reg.getF())) <<std::endl;
}

