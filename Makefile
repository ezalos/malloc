# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/20 16:46:57 by ezalos            #+#    #+#              #
#    Updated: 2021/01/14 13:42:33 by ezalos           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME		= libft_malloc_$(HOSTTYPE).so
NAME_LN		= libft_malloc.so
TESTOR		= malloc_testor.out
CC			= gcc

UNAME			:=	$(shell uname)
ifeq ($(UNAME),Linux)
LIB_THREAD = -pthread
else
LIB_THREAD =
endif

CFLAGS		= -Wall -Werror -Wextra

SO_FLAG		= -fPIC
SO_FLAG_	= -shared

# SO_FLAG	=
# SO_FLAG_	=

#For developping purposes:
# CFLAGS 		+= -fsanitize=address,undefined -g3
# CFLAGS 		+= -g

# SRCS_DIR	= srcs/
# HEAD_DIR	= includes/
# OBJS_DIR	= objs/
# OBJS_RBT_DIR	= objs/rbt/
# INCS		= $(wildcard $(HEAD_DIR)*.h)
# SRCS		= $(wildcard $(SRCS_DIR)*.c)
# SRCS		+= $(wildcard $(SRCS_DIR)rbt/*.c)

SRCS_DIR    = srcs/
SRCS_NAMES  = tree_available.c\
              tools.c\
              alloc_join.c\
              show_alloc_mem_utils.c\
              mem_type.c\
              alloc_sizes.c\
              show_alloc_mem_ex_colors.c\
              static_mem.c\
              alloc_init.c\
              alloc_move.c\
              show_alloc_mem_ex.c\
              alloc_get.c\
              zone_create.c\
              flag_set.c\
              calloc.c\
              alloc_split.c\
              print_numbers.c\
              realloc.c\
              free.c\
              flag_set_availability.c\
              malloc.c\
              show_alloc_mem_ex_zone.c\
              zone_delete.c\
              official.c\
              tree_unavailable.c\
              show_alloc_mem.c

#			  main_colors.c

SRCS_RBT_DIR    = rbt/
SRCS_RBT_NAMES  = tree_inorder.c\
                  tree_insert.c\
                  tree_insert_case.c\
                  tree_insert_repair.c\
                  tree_new_node.c\
                  tree_recoloring.c\
                  tree_remove_utils.c\
                  tree_print_node.c\
                  tree_inrorder.c\
                  tree_len.c\
                  tree_remove_delete_case_b.c\
                  tree_rotations.c\
                  tree_print.c\
                  tree_get.c\
                  tree_remove_delete_case_a.c\
                  tree_genealogy.c\
                  tree_free.c\
                  tree_remove.c\
                  tree_insert_recurse.c
HEAD_DIR    = includes/
HEAD_NAMES  = head.h\
              prototypes_malloc.h\
              tree.h\
              malloc_structs.h
OBJS_DIR    = objs/
OBJS_RBT_DIR    = objs/rbt/
$(shell mkdir -p $(OBJS_DIR))
$(shell mkdir -p $(OBJS_RBT_DIR))
INCS        = $(addprefix $(HEAD_DIR)/,$(HEAD_NAMES))
SRCS        = $(addprefix $(SRCS_DIR)/,$(SRCS_NAMES))
SRCS        += $(addprefix $(SRCS_DIR)/$(SRCS_RBT_DIR)/,$(SRCS_RBT_NAMES))

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
	ln -sf $(NAME) $(NAME_LN)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c $(INCS) Makefile
	$(CC) $(CFLAGS) $(SO_FLAG) -o $@ -c $< $(HEADERS)

$(TESTOR): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TESTOR) $(HEADERS) $(LIB_THREAD)

clean:
	rm -rf $(OBJS)
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME) $(NAME_LN)

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
