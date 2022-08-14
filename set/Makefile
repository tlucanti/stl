# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/14 11:26:42 by tlucanti          #+#    #+#              #
#    Updated: 2022/08/14 22:17:45 by tlucanti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ------------------------------ compiler options ------------------------------
NAME		=	test
NAME_SPEED	=	speed
CXX			=	g++ -std=c++98
CXXFLAGS	=	-Wall -Wextra -Wctor-dtor-privacy -Wnon-virtual-dtor \
					-Wold-style-cast -Woverloaded-virtual -Wsign-promo \
					-Wduplicated-branches -Wduplicated-cond -Wfloat-equal \
					-Wshadow=compatible-local -Wcast-qual -Wconversion \
					-Wzero-as-null-pointer-constant -Wextra-semi \
					-Wsign-conversion -Wlogical-op
CXXFLAGS	+= 	-Werror
CXXFLAGS	+= 	-pedantic-errors
CXXOPTIONS	=	-O3
# CXXOPTIONS	+=	-D__DEBUG
CC			=	CC

# --------------------------------- submodules ---------------------------------

# ------------------------------- linker options -------------------------------
LIBRARY		=	
OBJS_DIR	=	obj
INCLUDE_DIR	=	../include ../test ../iterator ../rb_tree ../pair
SCRS_DIR	=	.
DEPS_DIR	=	dep
# ------------------------------- project sorces -------------------------------
SRCS		=	\
				test_set

SPEED_SRCS	=	\
				speed_test

# ======================= UNCHANGEABLE PART OF MAKEFILE ========================
# ------------------------------------------------------------------------------
RM			=	rm -f
OBJS		=	$(addprefix ${OBJS_DIR}/,${SRCS:=.o})
SPEED_OBJS	=	$(addprefix ${OBJS_DIR}/,${SPEED_SRCS:=.o})
DEPS		=	$(addprefix ${DEPS_DIR}/,${SRCS:=.d})
SPEED_DEPS	=	$(addprefix ${DEPS_DIR}/,${SPEED_SRCS:=.d})
INCLUDE		=	$(addprefix -I, ${INCLUDE_DIR})
_NM_DEPS_MV	=	$(addprefix ${OBJS_DIR}/,${SRCS:=.d})
_SP_DEPS_MV	=	$(addprefix ${OBJS_DIR}/,${SPEED_SRCS:=.d})
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
all:			${OBJS_DIR} ${DEPS_DIR}
	@$(MAKE)	$(NAME)
	@$(MAKE)	$(NAME_SPEED)

# ------------------------------------------------------------------------------
$(NAME):		${OBJS}
	@echo [LD] Linking CXX Executable ${NAME}
	@${CXX}		-o ${NAME} ${CXXFLAGS} ${CXXOPTIONS} ${OBJS} ${LIBRARY}
	@printf		"$(G)TEST OK$(S)                                              \n"	

# ------------------------------------------------------------------------------
$(NAME_SPEED):	${SPEED_OBJS}
	@echo [LD] Linking CXX Executable ${NAME_SPEED}
	@${CXX}		-o ${NAME_SPEED} ${CXXFLAGS} ${CXXOPTIONS} ${SPEED_OBJS} ${LIBRARY}
	@printf		"$(G)SPEED OK$(S)                                            \n"	

-include ${DEPS}

# ------------------------------------------------------------------------------
${OBJS_DIR}/%.o: ${SCRS_DIR}/%.cpp Makefile
	@printf		"$(G)[${CC}]$(W)\t\tBuilding CXX object $(Y)$<$(S)$(SPACE)\r"
	@${CXX}		${CXXFLAGS} ${CXXOPTIONS} -c -MMD -MT $@ -o $@ $< ${INCLUDE}
	@mv			${@:.o=.d} ${DEPS_DIR}

# ------------------------------------------------------------------------------
clean:
	@printf		"$(R)[RM]$(W)\t\t${OBJS} ${DEPS}$(S)$(SPACE)\n"
	@printf		"$(R)[RM]$(W)\t\t${SPEED_OBJS} ${DEPS}$(S)$(SPACE)\n"
	@${RM}		${OBJS} ${DEPS}
	@${RM}		${SPEED_OBJS} ${DEPS}

# ------------------------------------------------------------------------------
fclean:			clean
	@printf		"$(R)[RM]$(W)\t\t${NAME}\n"
	@printf		"$(R)[RM]$(W)\t\t${NAME_SPEED}\n"
	@${RM}		${NAME}
	@${RM}		${NAME_SPEED}

# ------------------------------------------------------------------------------
re:				fclean all

# ------------------------------------------------------------------------------
__GXX_98_COMPILE: CXX = g++ -std=c++98
__GXX_98_COMPILE: CC = G++
__GXX_98_COMPILE: fclean $(NAME) $(NAME_SPEED)
__GXX_2a_COMPILE: CXX = g++ -std=c++2a
__GXX_2a_COMPILE: CC = G++
__GXX_2a_COMPILE: fclean $(NAME) $(NAME_SPEED)
__CLANG_98_COMPILE: CXX = clang++ -std=c++98
__CLANG_98_COMPILE: CC = CLANG++
__CLANG_98_COMPILE: fclean $(NAME) $(NAME_SPEED)
__CLANG_2a_COMPILE: CXX = clang++ -std=c++2a
__CLANG_2a_COMPILE: CC = CLANG++
__CLANG_2a_COMPILE: fclean $(NAME) $(NAME_SPEED)
__CXX_98_COMPILE: CXX = c++ -std=c++98
__CXX_98_COMPILE: CC = C++
__CXX_98_COMPILE: fclean $(NAME) $(NAME_SPEED)
__CXX_2a_COMPILE: CXX = c++ -std=c++2a
__CXX_2a_COMPILE: CC = C++
__CXX_2a_COMPILE: fclean $(NAME) $(NAME_SPEED)

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
	@mkdir		-p ${DEPS_DIR}

# ------------------------------------------------------------------------------
.PHONY:			all clean fclean re
