# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/08 14:54:30 by kostya            #+#    #+#              #
#    Updated: 2022/07/06 18:01:33 by tlucanti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ------------------------------ compiler options ------------------------------
NAME		=	ircserv
CXX			=	g++ -std=c++2a
CXXFLAGS	=	-Wall -Wextra -Werror -g3
CXXOPTIONS	=	-O0 -pedantic-errors
CC			=	CC
# --------------------------------- submodules ---------------------------------

# ------------------------------- linker options -------------------------------
LIBRARY		=	
OBJS_DIR	=	obj
INCLUDE_DIR	=	inc
SCRS_DIR	=	src
DEPS_DIR	=	dep
# ------------------------------- project sorces -------------------------------
SRCS		=	\
				main

# ======================= UNCHANGEABLE PART OF MAKEFILE ========================
# ------------------------------------------------------------------------------
RM			=	rm -f
OBJS		=	$(addprefix ${OBJS_DIR}/,${SRCS:=.o})
DEPS		=	$(addprefix ${DEPS_DIR}/,${SRCS:=.d})
INCLUDE		=	-I ${INCLUDE_DIR}
_DEPS_MV	=	$(addprefix ${OBJS_DIR}/,${SRCS:=.d})
LIBINIT		=	ar rc

W			=	\e[1;97m
G			=	\e[1;92m
Y			= 	\e[1;93m
R			=	\e[1;91m
C			=	\e[1;96m
P			=	\e[1;95m
S			=	\e[0m
SPACE		=	\t                     \t

# ------------------------------------------------------------------------------
all:			${OBJS_DIR} ${DEPS_DIR} vec
	@$(MAKE)	$(NAME)
	@printf		"$(G)OK$(S)                                                  \n"

# ------------------------------------------------------------------------------
$(NAME):		${OBJS}
	@printf		"$(C)[LD] $(W)Linking CXX Executable ${NAME}$(S)             \r"
	@${CXX}		-o ${NAME} ${CXXFLAGS} ${CXXOPTIONS} ${OBJS} ${LIBRARY}

-include ${DEPS}

# ------------------------------------------------------------------------------
${OBJS_DIR}/%.o: ${SCRS_DIR}/%.cpp Makefile
	@printf		"$(G)[${CC}]$(W)\t\tBuilding CXX object $(Y)$<$(S)           \r"
	@${CXX}		${CXXFLAGS} ${CXXOPTIONS} -c -MMD -MT $@ -o $@ $< ${INCLUDE}
	@mv			${@:.o=.d} ${DEPS_DIR}

# ------------------------------------------------------------------------------
clean:
	@printf		"$(R)[RM]$(W)\t\t${OBJS} ${DEPS}$(S)\n"
	@${RM}		${OBJS} ${DEPS}

# ------------------------------------------------------------------------------
fclean:			clean
	@printf		"$(R)[RM]$(W)\t\t${LIBRARY}\n"
	@printf		"$(R)[RM]$(W)\t\t${NAME}\n"
	@${RM}		${NAME}

# ------------------------------------------------------------------------------
vec:
	${CXX}		test/test_vector.cpp ${INCLUDE} -o tst
#	./tst

# ------------------------------------------------------------------------------
re:				fclean all

# ------------------------------------------------------------------------------
__GXX_98_COMPILE: CXX = g++ -std=c++98
__GXX_98_COMPILE: CC = G++
__GXX_98_COMPILE: fclean $(NAME)
__GXX_2a_COMPILE: CXX = g++ -std=c++2a
__GXX_2a_COMPILE: CC = G++
__GXX_2a_COMPILE: fclean $(NAME)
__CLANG_98_COMPILE: CXX = clang++ -std=c++98
__CLANG_98_COMPILE: CC = CLANG++
__CLANG_98_COMPILE: fclean $(NAME)
__CLANG_2a_COMPILE: CXX = clang++ -std=c++2a
__CLANG_2a_COMPILE: CC = CLANG++
__CLANG_2a_COMPILE: fclean $(NAME)
__CXX_98_COMPILE: CXX = c++ -std=c++98
__CXX_98_COMPILE: CC = C++
__CXX_98_COMPILE: fclean $(NAME)
__CXX_2a_COMPILE: CXX = c++ -std=c++2a
__CXX_2a_COMPILE: CC = C++
__CXX_2a_COMPILE: fclean $(NAME)

compile:
	$(MAKE) __GXX_98_COMPILE
	$(MAKE) __CLANG_98_COMPILE
	$(MAKE) __CXX_98_COMPILE
	$(MAKE) __GXX_2a_COMPILE
	$(MAKE) __CLANG_2a_COMPILE
	$(MAKE) __CXX_2a_COMPILE

# ------------------------------------------------------------------------------
${OBJS_DIR}:
	@printf		"$(P)[MK]\t$(Y)${OBJS_DIR}$(S)$(SPACE)\n"
	@mkdir		-p ${OBJS_DIR}

# ------------------------------------------------------------------------------
${DEPS_DIR}:
	@printf		"$(P)[MK]\t$(Y)${DEPS_DIR}$(S)$(SPACE)\n"
	mkdir		-p ${DEPS_DIR}

# ------------------------------------------------------------------------------
.PHONY:			all clean fclean re
