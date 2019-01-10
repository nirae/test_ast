# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/30 19:48:44 by ndubouil          #+#    #+#              #
#    Updated: 2019/01/06 23:10:55 by ndubouil         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Binaries and Flags

CC		=	/usr/bin/gcc
RM		=	/bin/rm
CFLAGS	=	-Wall -Wextra -Werror

# Directories

LIBFT	=	./libft/
ILIBFT	=	./$(LIBFT)includes/
SRC		=	./
P		=	$(SRC)parsing/
ENV		=	$(SRC)environment/
BT		=	$(SRC)builtins/
CD		=	$(BT)cd/
ENVBT	=	$(BT)env/

#  Files

MAIN	=	$(SRC)main.c
SRCS	=	$(MAIN)

OBJ		=	$(patsubst %.c,%.o,$(SRCS))

# Name of the project

NAME	=	minishell

.PHONY: all clean fclean re

# Rules

all:		$(NAME)
		@true

$(NAME):	Makefile $(OBJ) $(MAIN) $(LIBFT)
		@echo "Compiling Libft ..."
		@make -C $(LIBFT)
		@echo "Building $(NAME) ..."
		@$(CC) $(CFLAGS) $(OBJ) -I$(ILIBFT) -L$(LIBFT) -lft -o $(NAME)
		@echo "I'm READY"

%.o: 		%.c
		@echo "Creating $@ ..."
		@$(CC) $(CFLAGS) -I$(ILIBFT) -c $< -o $@

clean:
		@echo "Cleaning Objs ..."
		@$(RM) -f $(OBJ)
		@echo "Cleaning Libft Objs ..."
		@make clean -C $(LIBFT)

fclean:		clean
		@echo "Cleaning $(NAME) exec"
		@$(RM) -f $(NAME)
		@echo "Cleaning libft exec"
		@make fclean -C $(LIBFT)

re:			fclean all
