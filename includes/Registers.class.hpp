/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Registers.class.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhismans <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/20 16:55:53 by hhismans          #+#    #+#             */
/*   Updated: 2016/10/12 01:23:31 by clement          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REGISTERS_CLASS_HPP
# define REGISTERS_CLASS_HPP

# include <inttypes.h> //Allow uint8_t on Debian
# include <iostream>

# define DEFAULT_PC 0x150
# define DEFAULT_SP 0xFFFE

namespace Gbmu {

	class Registers {
		public:
			//Registers &	operator=( Registers const & rhs);
			Registers( void );
			//Registers const ( Registers const & src );
			~Registers( void );
			//getter
			uint8_t		getA() const;
			uint8_t		getB() const;
			uint8_t		getC() const;
			uint8_t		getD() const;
			uint8_t		getE() const;
			uint8_t		getH() const;
			uint8_t		getL() const;
			uint8_t		getF() const;
			bool		getFz() const;
			bool		getFn() const;
			bool		getFh() const;
			bool		getFc() const;
			uint16_t	getAF() const;
			uint16_t	getBC() const;
			uint16_t	getDE() const;
			uint16_t	getHL() const;
			uint16_t	getPC() const;
			uint16_t	getSP() const;

			//setter
			void		setA(uint8_t value);
			void 		setB(uint8_t value);
			void 		setC(uint8_t value);
			void 		setD(uint8_t value);
			void 		setE(uint8_t value);
			void 		setH(uint8_t value);
			void 		setL(uint8_t value);
			void 		setF(uint8_t value);
			void 		setAF(uint16_t value);
			void 		setBC(uint16_t value);
			void 		setDE(uint16_t value);
			void 		setHL(uint16_t value);
			void 		setFz(bool value);
			void 		setFn(bool value);
			void 		setFh(bool value);
			void 		setFc(bool value);
			void		setPC(uint16_t addr);
			void		setSP(uint16_t addr);

		private:

			uint8_t		_A;
			uint8_t		_B;
			uint8_t		_C;
			uint8_t		_D;
			uint8_t		_E;
			uint8_t		_F;
			uint8_t		_H;
			uint8_t		_L;
			uint16_t	_pc;
			uint16_t	_sp;
	};

}

std::ostream &operator<<(std::ostream &out, const Gbmu::Registers &reg);

#else
namespace Gbmu {
	class Registers;
}
#endif

