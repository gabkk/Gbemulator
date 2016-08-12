# include "../Gbmu.class.hpp"
# include "Gb.class.hpp"
# include "Cartridge.class.hpp"

int						main(int argc, const char **argv)
{
	Gbmu::Gb			gb;
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

	
	return(0);
}
