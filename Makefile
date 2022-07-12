NAME = ircserv

SRCS = srcs/main.cpp

CXX = c++

OBJS = ${SRCS:.cpp=.o}

INCL_DIR = includes/

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

all : $(NAME)

.cpp.o :
		${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

$(NAME): ${OBJS}
		${CXX} -o ${NAME} ${OBJS}

clean :
		rm -rf ${OBJS}

fclean: clean
		rm -rf ${NAME}

re:		fclean all

.PHONY: all clean fclean res
