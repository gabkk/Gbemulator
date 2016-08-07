#include "../Gbmu.class.hpp"

int				main(int argc, const char **argv)
{
	Gb			gb;
	
	if (argc != 2)
	{
		std::cout << "Gbmu Should take a cartridge as parameter and can't take more than 1 cartridge" << std::endl;
		return(0);
	}
	try
	{
		gb.load(argv[1]);
	}
	catch (std::exception& e)
	{
		std::perror("File opening failed");
		return (false);
	}
	return(0);
}
