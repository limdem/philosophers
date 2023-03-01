RM = rm -f
CFLAGS = -Wall -Wextra -Werror
NAME = philo_bonus
INCLUDES = includes/philo.h
SRCS_DIR = srcs/
SRCS = actions.c\
	   actions2.c\
       handle_diner.c\
	   handle_diner2.c\
	   handle_diner3.c\
	   main.c\
	   utils.c\
	   utils2.c
SRCS_PATH = $(addprefix $(SRCS_DIR), $(SRCS))
OBJS = $(SRCS:.c=.o)

%.o: %.c
	${CC} -c ${CFLAGS} ${INCLUDES} $< -o $@ -lpthread

all: $(NAME)

$(NAME) : $(OBJS)
		gcc -o $(NAME) $(OBJS) -lpthread

$(OBJS): $(SRCS_PATH)
	gcc -c ${CFLAGS} ${CFLAGS} ${INCLUDES} ${SRCS_PATH}

all : $(NAME)

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY: fclean re all clean