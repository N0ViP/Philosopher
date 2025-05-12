#include "philo.h"

int	check_philos(t_philo *philos, int i)
{
	struct timeval	tv;
	long long		time;
	int 			t_to_die;

	t_to_die = philos[i].stuff->t_to_die;
	pthread_mutex_lock(&philos[i].time_protection);
	time = time_ms(&philos[i].tv_beg);
	pthread_mutex_unlock(&philos[i].time_protection);
	gettimeofday(&tv, NULL);
	if (time_ms(&tv) - time >= (long long)t_to_die)
	{
		kill_philos(philos, philos[i].stuff->number_of_philos);
		gettimeofday(&tv, NULL);
		print_message(&philos[i].stuff->tv_start, &tv, i + 1, "is died\n");
		return (false);
	}
	return (true);
}

int	monitoring_1(t_philo *philos)
{
	int i;
	int	n;

	n = philos[0].stuff->number_of_philos;
	while (true)
	{
		i = 0;
		while (i < n)
		{
			if (!check_philos(philos, i))
				return (1);
			i++;
		}
		// usleep(100);
	}
	return (0);
}

int	monitoring_2(t_philo *philos)
{
	int i;
	int	cnt;
	int	tmp;

	tmp = philos[0].stuff->number_of_times_each_philo_must_eat;
	while (true)
	{
		i = 0;
		cnt = 0;
		while (i < philos[i].stuff->number_of_philos)
		{
			if (!check_philos(philos, i))
				return (1);
			pthread_mutex_lock(&philos[i].eat_protection);
			if (philos[i].eat >= tmp)
				cnt++;
			pthread_mutex_unlock(&philos[i].eat_protection);
			i++;
		}
		if (cnt == philos[0].stuff->number_of_philos)
			return (0);
		usleep(500);
	}
	return (0);
}

void	*monitoring(void *arg)
{
	t_philo		*philos;
	long long	reval;

	philos = (t_philo *) arg;
	if (!philos->stuff->number_of_times_each_philo_must_eat)
		reval = monitoring_1(philos);
	else
		reval = monitoring_2(philos);
	return ((void *)(reval));
}