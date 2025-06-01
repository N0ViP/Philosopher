#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef struct s_stuff
{
	struct timeval	tv_start;
	sem_t			*forks;
	pid_t			parent_pid;
	pid_t			*philos;
	int				number_of_philos;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	int				must_eat;
} t_stuff;

typedef struct t_philo
{
	pthread_mutex_t	eat_protection;
	pthread_mutex_t	alive_protection;
	pthread_mutex_t	time_protection;
	struct timeval	tv_beg;
	t_stuff			*stuff;
	int				eat;
	bool			alive;
}	t_philo;

#endif