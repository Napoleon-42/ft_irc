# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/22 17:56:55 by lnelson           #+#    #+#              #
#    Updated: 2022/07/12 18:06:28 by lnelson          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#							FT_IRC MAKEFILE									   #
# **************************************************************************** #

NAME = server

SRCS = srcs/main.cpp

CXX = c++

OBJS = ${SRCS:.cpp=.o}

INCL_DIR = includes/

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 -I ${INCL_DIR}

all : $(NAME)

std: ${NAME_STD}

INCL		= ${INCL_DIR}ft_irc.hpp

SRCS		=	srcs/main.cpp \
				srcs/Client.cpp \
				srcs/Channel.cpp \
				srcs/commands/Help.cpp \
				srcs/commands/Join.cpp \
				srcs/commands/ChannelBan.cpp \
				srcs/commands/Nick.cpp \
				srcs/commands/List.cpp \
				srcs/commands/Oper.cpp

OBJS		= ${SRCS:.cpp=.o}

.cpp.o :
		${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

$(NAME): ${OBJS}
		${CXX} ${CXXFLAGS} -o ${NAME} ${OBJS}

clean :
		rm -rf ${OBJS}

fclean: clean
		rm -rf ${NAME}

re:		fclean all

.PHONY: all clean fclean res
