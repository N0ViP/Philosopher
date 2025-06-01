#include "philo_bonus.h"

static void	one_philo(void)
{
	printf("0\t1\tis thinking\n");
	printf("0\t1\thas taken a fork\n");
	printf("0\t1\tdied\n");
}

static bool	init_each_philo(t_philo *philo, t_stuff *stuff, int i)
{
	philo->stuff = stuff;
	philo->alive = true;
	philo->eat = 0;
	philo->tv_beg = (struct timeval){0};
	if (pthread_mutex_init(&philo->eat_protection, NULL))
		return (false);
	if (pthread_mutex_init(&philo->time_protection, NULL))
		return (false);
	if (pthread_mutex_init(&philo->alive_protection, NULL))
		return (false);
	return (true);
}

static bool	allocate_stuff(t_stuff *stuff, t_philo **philos)
{
	stuff->philos = malloc(sizeof(pid_t) * stuff->number_of_philos);
	if (!stuff->philos)
		return (false);
	stuff->forks = sem_open("/forks", O_CREAT, 0777, stuff->number_of_philos);
	if (!stuff->forks)
		return (free(stuff->philos), false);
    *philos = malloc(sizeof(t_philo) * stuff->number_of_philos);
	if (!stuff->forks)
		return (free(stuff->philos), sem_close(stuff->forks), sem_unlink("/forks"), false);
	return (true);
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
		i++;
	}
}

static bool	init_philos(t_stuff *stuff)
{
    t_philo			*philos;
	int				i;
	int				reval;
	int				status;

	i = 0;
	if (!allocate_stuff(stuff, &philos))
		return (false);
	gettimeofday(&stuff->tv_start, NULL);
	while (i < stuff->number_of_philos)
	{
		if (!init_each_philo(&philos[i], stuff, i))
			return (destroy_mutex(philos, i), free(philos), free(stuff->philos), sem_close(stuff->forks), sem_unlink("/forks"), false);
		i++;
	}
	i = 0;
	while (i < stuff->number_of_philos)
	{
		stuff->philos[i] = fork();
		if (!stuff->philos[i])
		{
			run_sumilation(&philos[i]);
			break;
		}
		if (stuff->philos[i] == -1)
			return (kill_philos(philos, i), free(philos), free(stuff->philos), sem_close(stuff->forks), sem_unlink("/forks"), false);
		i++;
	}
	reval = true;
	if (stuff->parent_pid == getpid())
	{
		i = 0;
		while (i < stuff->number_of_philos)
		{
			waitpid(stuff->philos[i], &status, 0);
			if (!status)
				reval = false;
		}
	}
	return (kill_philos(philos, i), free(philos), free(stuff->philos), sem_close(stuff->forks), sem_unlink("/forks"), reval);
}

int main(int ac, char **av)
{
    t_stuff	stuff;
	int		reval;

	if (ac != 5 && ac != 6)
		return (write(2, "Invalid arguments!\n", 19), 1);
	stuff.parent_pid = getpid();
	stuff.must_eat = 0;
	stuff.number_of_philos = ft_atoi(av[1]);
	stuff.t_to_die = ft_atoi(av[2]);
	stuff.t_to_eat = ft_atoi(av[3]);
	stuff.t_to_sleep = ft_atoi(av[4]);
	if (stuff.number_of_philos <= 0
		|| stuff.t_to_die <= 0
		|| stuff.t_to_eat <= 0
		|| stuff.t_to_sleep <= 0)
		return (write(2, "Invalid arguments!\n", 19), 1);
	if (ac == 6)
	{
		stuff.must_eat = ft_atoi(av[5]);
		if (stuff.must_eat <= 0)
			return (write(2, "Invalid arguments!\n", 19), 1);
	}
	if (stuff.number_of_philos == 1)
		return (one_philo(), 0);
    reval = init_philos(&stuff);
	return (!reval);
}
