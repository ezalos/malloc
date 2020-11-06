# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/20 16:46:57 by ezalos            #+#    #+#              #
#    Updated: 2020/11/03 11:15:43 by ezalos           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME		= libft_malloc_$(HOSTTYPE).so
TESTOR		= malloc_testor.out
CC			= gcc

CFLAGS		= -Wall -Werror -Wextra

SO_FLAG		= -fPIC
SO_FLAG_	= -shared

# SO_FLAG	=
# SO_FLAG_	=

#For developping purposes:
# CFLAGS 		+= -fsanitize=address,undefined -g3
# CFLAGS 		+= -g

SRCS_DIR	= srcs/
HEAD_DIR	= includes/
OBJS_DIR	= objs/
OBJS_RBT_DIR	= objs/rbt/
$(shell mkdir -p $(OBJS_DIR))
$(shell mkdir -p $(OBJS_RBT_DIR))

INCS		= $(wildcard $(HEAD_DIR)*.h)
SRCS		= $(wildcard $(SRCS_DIR)*.c)
SRCS		+= $(wildcard $(SRCS_DIR)rbt/*.c)
OBJS		= $(SRCS:$(SRCS_DIR)%.c=$(OBJS_DIR)%.o)

HEADERS		= -I./$(HEAD_DIR)

##########################
##						##
##		  BASIC			##
##						##
##########################


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(SO_FLAG_) -o $(NAME) $(HEADERS)
	ln -sf $(NAME) libft_malloc.so

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c $(INCS) Makefile
	$(CC) $(CFLAGS) $(SO_FLAG) -o $@ -c $< $(HEADERS)

$(TESTOR): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TESTOR) $(HEADERS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re : fclean
	$(MAKE) all

##########################
##						##
##		  PERSO			##
##						##
##########################

SUPPORTED_COMMANDS := run tests
SUPPORTS_MAKE_ARGS := $(findstring $(firstword $(MAKECMDGOALS)), $(SUPPORTED_COMMANDS))
ifneq "$(SUPPORTS_MAKE_ARGS)" ""
  COMMAND_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(COMMAND_ARGS):;@:)
endif

run: $(TESTOR)
	./$(TESTOR) $(COMMAND_ARGS)

UNAME			:=	$(shell uname)
ifeq ($(UNAME),Linux)
tests: $(NAME)
	sh .tmp/script_linux.sh $(COMMAND_ARGS)
else
tests: $(NAME)
	sh .tmp/script_macos.sh $(COMMAND_ARGS)
endif

prototypes:
	python3 .tmp/prototype_catcher.py srcs includes/prototypes_malloc.h malloc

REQUEST 		= 'read -p "Enter a commit message:	" pwd && echo $$pwd'
COMMIT_MESSAGE ?= $(shell bash -c $(REQUEST))
git :
		git add -A
		git status
		(echo -n "Are you sure? [y/N] " && read ans && [ $${ans:-N} = y ])
		git commit -m "$(COMMIT_MESSAGE)"
		git push

.PHONY: clean fclean
