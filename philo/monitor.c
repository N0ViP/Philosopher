#include "philo.h"

int	monitorint_1(t_philo *philos)
{
	int i;
	int	n;

	n = philos[0].stuff->number_of_philos;
	while (true)
	{
		i = 0;
		while (i < n)
		{
			if (check_philos(&philos[i]))
				return (1);
			i++;
		}
		usleep(5000);
	}
	return (0);
}

int	monitorint_2(t_philo *philos)
{
	int i;
	int	cnt;
	int	tmp;

	tmp = philos[0].stuff->number_of_times_each_philo_must_eat;
	while (true)
	{
		i = 0;
		cnt = 0;
		while (i < philos[0].stuff->number_of_philos)
		{
			if (check_philos(&philos[i]))
				return (1);
			pthread_mutex_lock(&philos[i].eat_protection);
			if (philos[i].eat >= tmp)
				cnt++;
			pthread_mutex_unlock(&philos[i].eat_protection);
			i++;
		}
		if (cnt == philos[0].stuff->number_of_philos)
			return (kill_philos(philos, philos[0].stuff->number_of_philos), 0);
		usleep(5000);
	}
	return (0);
}

void	*monitoring(void *arg)
{
	t_philo	*philos;
	int		reval;

	philos = (t_philo *) arg;
	if (!philos->stuff->number_of_times_each_philo_must_eat)
		reval = monitoring_1(philos);
	else
		reval = monitoring_2(philos);
	return (&reval);
}