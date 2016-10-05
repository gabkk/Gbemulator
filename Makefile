NAME = Gbmu

SRCS_DIR = srcs/
OBJ_DIR = obj/
INC_DIR = includes/

CXX = clang++
CFLAGS = -Wall -Wextra -c -std=c++11
IFLAGS = -I $(INC_DIR)

INC_FILES = Cartridge.class.hpp \
			Cpu.class.hpp \
			Gb.class.hpp \
			Memory.class.hpp \
			Registers.class.hpp \
			Instructions.class.hpp

SRCS_FILES =  main.cpp \
			  Gb.cpp \
			  Cartridge.cpp \
			  Cpu.cpp \
			  Memory.cpp \
			  Registers.cpp \
			  Instructions.cpp

OBJ_FILES = $(SRCS_FILES:.cpp=.o)

OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))
INC = $(addprefix $(INC_DIR), $(INC_FILES))

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(IFLAGS) -o $@ $^ $(LFLAGS)

# @$ is the target (here $(OBJ_DIR))
$(OBJ_DIR):
	mkdir -p $@

# $@ is the target, $< iterates over each dependency
$(OBJ_DIR)%.o: $(SRCS_DIR)%.cpp $(INC)
	$(CXX) $(CFLAGS) $(IFLAGS) -o $@ $<

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
