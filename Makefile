# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/17 13:36:31 by vileleu           #+#    #+#              #
#    Updated: 2025/07/17 13:40:59 by vileleu          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLUE		= \033[0;34m
GREEN		= \033[0;32m
LIGHTBLUE	= \033[1;34m
RED			= \033[0;31m
YELLOW		= \033[1;33m
ORANGE		= \033[0;33m
MAGENTA		= \033[0;35m
RESET		= \033[0m

DIR_SRCS	= srcs
DIR_INCS	= includes
DIR_OBJS	= objects
DIR_DEPS 	= dependencies

SRCS		=	main.c

INCS		= -I $(DIR_INCS)
OBJS 		= $(patsubst %.c,$(DIR_OBJS)/%.o,$(SRCS))
DEPS 		= $(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$(OBJS))

NAME		= ft_nmap
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
OFLAGS		= -MMD -MP
RM			= rm -rf
MKDIR		= $(shell mkdir -p $(DIR_OBJS) $(DIR_DEPS))

$(MKDIR)

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c
			@printf "\n$(BLUE)$< -> $(ORANGE)$@$(RESET)"
			@$(CC) $(CFLAGS) $(OFLAGS) $(INCS) -c $< -o $@
			@mv $(addsuffix .d,$(basename $@)) $(DIR_DEPS)

$(NAME):	$(OBJS)
			@printf "\n\n$(BLUE)Compiling $(LIBFT) ... $(RESET)"
			@printf "$(GREEN)[✔]$(RESET)"
			@printf "\n\n$(BLUE)Compiling $(NAME) ... $(RESET)"
			@$(CC) $(CFLAGS) $(OBJS) $(INCS) $(LIBFT) -o $(NAME)
			@printf "$(GREEN)[✔]\n[$(NAME) done]$(RESET)\n\n"

all:		 $(NAME)

restore_directory:
			$(MKDIR)

clean:
			@printf "\n$(BLUE)Clean libraries ..."
			@printf "$(GREEN) [✔]$(RESET)"
			@printf "\n\n$(BLUE)Clean objects and dependencies ..."
			@$(RM) $(DIR_OBJS) $(DIR_DEPS)
			@printf "$(GREEN) [✔]$(RESET)\n\n"

fclean:		clean
			@printf "$(BLUE)Delete $(LIBFT) ..."
			@printf "$(GREEN) [✔]$(RESET)\n\n"
			@printf "$(BLUE)Delete $(NAME) ..."
			@$(RM) $(NAME)
			@printf "$(GREEN) [✔]$(RESET)\n\n"

re:			fclean restore_directory all

.PHONY:		all restore_directory clean fclean re