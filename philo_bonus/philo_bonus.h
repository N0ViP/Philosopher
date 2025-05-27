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

typedef struct s_stuff
{
	pid_t			*philos;
	sem_t			*forks;
	int				number_of_philos;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	int				must_eat;
	struct timeval	tv_start;
} t_stuff;

typedef struct t_philo
{
	t_stuff			*stuff;
	struct timeval	tv_beg;
	bool			alive;
	int				eat;
	pthread_mutex_t	eat_protection;
	pthread_mutex_t	alive_protection;
	pthread_mutex_t	time_protection;
}	t_philo;

#endif