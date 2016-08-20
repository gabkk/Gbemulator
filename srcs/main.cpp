# include "../Gbmu.class.hpp"
# include "Registers.class.hpp"
# include <iostream>

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
