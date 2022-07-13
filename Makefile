# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/22 17:56:55 by lnelson           #+#    #+#              #
#    Updated: 2022/06/27 15:34:20 by lnelson          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#							FT_IRC MAKEFILE									   #
# **************************************************************************** #

NAME		= ft_irc

SRCS_DIR	= srcs/

OBJS_DIR	= objs/

INCL_DIR	= includes/

CC			= c++

CFLAGS		= -Wall -Wextra -Werror -std=c++98 -I ${INCL_DIR}

FLAGS		= 

# **************************************************************************** #

INCL		= ${INCL_DIR}ft_irc.hpp

SRCS		=	srcs/main.cpp \
				srcs/utils.cpp \
				srcs/Client.cpp \
				srcs/Channel.cpp \
				srcs/commands/Help.cpp \
				srcs/commands/Join.cpp \
				srcs/commands/ChannelBan.cpp \
				srcs/commands/Nick.cpp \
				srcs/commands/Oper.cpp



# **************************************************************************** #

OBJS		= ${SRCS:.cpp=.o}

.cpp.o :
	${CC} ${CFLAGS} -c $< -o ${<:.cpp=.o}

${NAME}:	${OBJS} ${INCL}
		${CC} -o ${NAME} ${OBJS} ${FLAGS} 

###############################################################################

all:	${NAME}


clean:	
		rm ${OBJS}

fclean:	clean
		rm ${NAME}

re:	fclean all