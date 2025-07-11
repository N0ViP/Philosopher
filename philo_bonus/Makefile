CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror -g
SOURCE	=	clean.c	ft_strjoin.c	init_stuff.c	philo_bonus.c	utils_2.c		\
			ft_atoi.c	ft_strlen.c	monitor.c	run_simulation.c	wait_child.c	\
			ft_itoa.c	init_philos.c	one_philo.c	utils_1.c
NAME	=	philo_bonus
OBJS	=	$(SOURCE:.c=.o)

all: $(NAME)

%.o:%.c	$(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re