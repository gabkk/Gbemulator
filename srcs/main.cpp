# include "../Gbmu.class.hpp"
# include "Cartridge.class.hpp"
# include "Gb.class.hpp"
# include "Cpu.class.hpp"


static void		s_set_cartridge(Cpu cpu, std::string path)
{
	Gb::Model	Auto;
	Cartridge	cartridge(&cpu, path, Auto);

}

int				main(int argc, const char **argv)
{
	Cpu			cpu;
	std::string path;

	if (argc != 2)
	{
		std::cout << "Gbmu Should take a cartridge as parameter and can't take more than 1 cartridge" << std::endl;
		return(0);
	}
	path = argv[1];
	s_set_cartridge(cpu, path);
	return(0);
}
