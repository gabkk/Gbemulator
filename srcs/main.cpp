# include "../Gbmu.class.hpp"
# include "Registers.class.hpp"
# include <iostream>

	std::ostream &operator<<(std::ostream &out, const Registers &reg){
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
					<< "regHL = 0x" << std::hex << unsigned(reg.getHL()) <<std::endl;
	}
int						main()
{

	Registers	reg;
	uint8_t 	t = 8;
	t = reg.getA();
	std::cout << reg;
	reg.setAF(0x1234);
	reg.setBC(0x1234);
	reg.setDE(0x1234);
	reg.setHL(0x1234);
	std::cout << reg;


/*	Gbmu::Gb			gb;
	std::string 		path;

	if (argc != 2)
	{
		std::cout << "Gbmu Should take a cartridge as parameter and can't take more than 1 cartridge" << std::endl;
		return(0);
	}
	path = argv[1];
		
	try
	{
		gb.load(path);
	}
	catch (std::exception& e)
	{
		std::perror("File opening failed");
	}
	*/
	return(0);
}
