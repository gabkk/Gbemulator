
NAME = Gbmu

CFLAGS = -Wall -Wextra -Werror -c

LFLAGS = -lncurses -Wall -std=c11

INC = -I includes/

SRCSDIR = srcs/

SRCS =  main.cpp \
		Gb.cpp \

OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	clang++ $(INC) -o  $(NAME) $(OBJ) $(LFLAGS)

$(OBJ): $(addprefix $(SRCSDIR),$(SRCS))
	clang++ $(CFLAGS) $(INC) $(addprefix $(SRCSDIR),$(SRCS))

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: nof
