#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <semaphore.h>

typedef struct s_stuff
{
	struct timeval	tv_start;
	struct timeval	tv_beg;
	sem_t			*forks;
	sem_t			*time_protection;
	sem_t			*alive_protection;
	sem_t			*eat_protection;
	sem_t			*lock;
	pid_t			*philos;
	pid_t			philo_id;
	pid_t			p_pid;
	char			*sem_time_name;
	char			*sem_eat_name;
	char			*sem_alive_name;
	bool			alive;
	int				number_of_philos;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	int				must_eat;
} t_stuff;

#endif