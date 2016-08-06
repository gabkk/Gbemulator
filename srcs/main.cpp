#include "../Gbmu.class.hpp"

int				main(int argc, const char **argv)
{
	Gb			gb;
	
	if (argc != 2)
	{
		std::cout << "Gbmu can't take more than 1 cartridge" << std::endl;
		return(0);
	}
	gb.load(argv[1]);
	return(0);
}
