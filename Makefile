# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/17 13:36:31 by vileleu           #+#    #+#              #
#    Updated: 2025/08/26 21:46:11 by vileleu          ###   ########.fr        #
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

SRCS		=	main.c opt.c init_scan_configuration.c \
				parsing/parsing.c parsing/parsing_opt.c parsing/parsing_arg.c parsing/parsing_list.c parsing/parsing_utils.c parsing/parsing_error.c \
				scan/scan_send.c scan/scan_receive.c scan/scan_tcp.c scan/scan_udp.c scan/scan_utils.c scan/scan_error.c

INCS		= -I $(DIR_INCS)
OBJS 		= $(patsubst %.c,$(DIR_OBJS)/%.o,$(SRCS))
DEPS 		= $(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$(OBJS))

NAME		= ft_nmap
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -pthread -lpcap -g3 -fsanitize=address
OFLAGS		= -MMD -MP -MF $(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$@)
RM			= rm -rf

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c
			@mkdir -p $(dir $@) $(patsubst $(DIR_OBJS)/%,$(DIR_DEPS)/%,$(dir $@))
			@printf "\n$(BLUE)$< -> $(ORANGE)$@ $(BLUE)-> $(ORANGE)$(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$@)$(RESET)"
			@$(CC) $(CFLAGS) $(OFLAGS) $(INCS) -c $< -o $@

$(NAME):	$(OBJS)
			@printf "\n\n$(BLUE)Compiling $(NAME) ... $(RESET)"
			@$(CC) $(CFLAGS) $(OBJS) $(INCS) -o $(NAME)
			@printf "$(GREEN)[✔]\n[$(NAME) done]$(RESET)\n\n"

-include	$(DEPS)

all:		 $(NAME)

install:
			@sudo apt update
			@sudo apt upgrade
			@sudo apt install libpcap-dev

clean:
			@printf "\n$(BLUE)Clean libraries ..."
			@printf "$(GREEN) [✔]$(RESET)"
			@printf "\n\n$(BLUE)Clean objects and dependencies ..."
			@$(RM) $(DIR_OBJS) $(DIR_DEPS)
			@printf "$(GREEN) [✔]$(RESET)\n\n"

fclean:		clean
			@printf "$(BLUE)Delete $(NAME) ..."
			@$(RM) $(NAME)
			@printf "$(GREEN) [✔]$(RESET)\n\n"

re:			fclean all

.PHONY:		all clean fclean re