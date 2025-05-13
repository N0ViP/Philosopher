# include "philo.h"

long long	time_ms(struct timeval *tv)
{
 	return ((long long)(tv->tv_sec * 1000LL) + (tv->tv_usec / 1000));
}

long long	time_us(struct timeval *tv)
{
 	return ((long long)(tv->tv_sec * 1000000LL) + tv->tv_usec);
}

void	kill_philos(t_philo *philos, int n_of_philos)
{
	int	i;

	i = 0;
	while (i < n_of_philos)
	{
		pthread_mutex_lock(&philos[i].alive_protection);
		philos[i].alive = 0;
		pthread_mutex_unlock(&philos[i].alive_protection);
		i++;
	}
	i = 0;
	while (i < n_of_philos)
	{
		pthread_join(philos[i].stuff->philos[i], NULL);
		i++;
	}
	destroy_mutex(philos, n_of_philos);
}

void	destroy_mutex(t_philo *philos, int n_of_philos)
{
	int	i;

	i = 0;
	while (i < n_of_philos)
	{
		pthread_mutex_destroy(&philos[i].eat_protection);
		pthread_mutex_destroy(&philos[i].alive_protection);
		pthread_mutex_destroy(&philos[i].time_protection);
		pthread_mutex_destroy(&philos[i].stuff->forks[i]);
		i++;
	}
}

void	ft_usleep(t_philo *philo, int time)
{
	struct timeval	tv_before;
	struct timeval	tv_after;
	int				alive;

	gettimeofday(&tv_before, NULL);
	while (true)
	{
		pthread_mutex_lock(&philo->alive_protection);
		alive = philo->alive;
		pthread_mutex_unlock(&philo->alive_protection);
		if (!alive)
			return ;
		gettimeofday(&tv_after, NULL);
		if (time_ms(&tv_after) - time_ms(&tv_before) >= time)
			break ;
		usleep(500);
	}
}
