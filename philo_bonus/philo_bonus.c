#include "philo_bonus.h"

void	clean_up(t_stuff *stuff)
{
	sem_close(stuff->lock);
	sem_close(stuff->forks);
	sem_unlink("/forks");
	sem_unlink("/lock");
	free(stuff->philos);
}

void one_philo(int t_to_die)
{
	pid_t	pid;
	sem_t	*sem;
	int		ex_status;

	unlink("/forks");
	sem = sem_open("/forks", O_CREAT, 0777);
	pid = fork();
	if (pid == 0)
	{
		printf("%d\t%d\tis thinking\n", 1, 0);
		printf("%d\t%d\thas taken a fork\n", 1, 0);
		usleep(1000 * t_to_die);
		printf("%d\t%d\tdied\n", 1, t_to_die);
	}
	else
	{
		waitpid(pid, ex_status, 0);
	}
	exit(EXIT_SUCCESS);
}

void	kill_philos(t_stuff *stuff, int n_of_philos)
{
	int	i;

	i = 0;
	while (i < n_of_philos)
	{
		kill(stuff->philos[i], SIGKILL);
		i++;
	}
	clean_up(stuff);
}

void	allocate_philos_forks(t_stuff *stuff)
{
	stuff->p_pid = getpid();
	stuff->philos = malloc (sizeof(pid_t) * stuff->number_of_philos);
	if (!stuff->philos)
	{
		exit (EXIT_FAILURE);
	}
	sem_unlink("/forks");
	sem_unlink("/lock");
	stuff->lock = sem_open("/lock", O_CREAT, 0777, 1);
	stuff->forks = sem_open("/forks", O_CREAT, 0777, stuff->number_of_philos);
	if (!stuff->forks || !stuff->lock)
	{
		clean_up(stuff);
		exit(EXIT_FAILURE);
	}
	sem_wait(stuff->lock);
}

void	init_time(t_stuff *stuff)
{
	gettimeofday(&stuff->tv_start, NULL);
	gettimeofday(&stuff->tv_beg, NULL);
}

void	run_philos(t_stuff *stuff)
{
	int	i;

	i = 0;
	while (i < stuff->number_of_philos)
	{
		if (getpid() != stuff->p_pid)
			break ;
		stuff->philo_id = i;
		stuff->philos[i] = forks();
		if (stuff->philos[i] == -1)
		{
			kill_philos(stuff, i);
			exit(1);
		}
		i++;
	}
	i = 0;
	if (getpid() == stuff->p_pid)
	{
		while (i < stuff->philos)
		{
			sem_post(stuff->lock);
			i++;
		}
	}
}

void	philos_died(t_stuff *stuff, int pid)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	kill_philos(stuff, stuff->number_of_philos);
	printf("%lld\t%d\tdied\n", time_ms(&tv) - \
		time_ms(&stuff->tv_start), pid);
	exit(EXIT_FAILURE);
}

void	wait_child(t_stuff *stuff)
{
	int	exit_child_val;
	int	i;

	i = 1;
	while (true)
	{
		waitpid(-1, &exit_child_val, 0);
		if (exit_child_val != 0)
		{
			philo_died(stuff, exit_child_val);
		}
		if (stuff->must_eat != 0 && i == stuff->number_of_philos)
		{
			kill_philos(stuff, stuff->number_of_philos);
			exit(EXIT_SUCCESS);
		}
		i++;
	}
}

void	start(void *arg)
{
	t_stuff	*stuff;

	stuff = (t_stuff *) arg;
	sem_wait(stuff->lock);
	while (is_alive(stuff))
	{
		thinking(stuff);
		eating(stuff);
		sleeping(stuff);
	}
}

void	monitor(t_stuff *stuff)
{
	;
}

void init_mutexs(t_stuff *stuff)
{
	if (pthread_mutex_init(&stuff->alive_protection, NULL))
	{
		clean_up(stuff);
		exit(EXIT_FAILURE);
	}
	if (pthread_mutex_init(&stuff->eat_protection, NULL))
	{
		pthread_mutex_destroy(&stuff->alive_protection);
		clean_up(stuff);
		exit(EXIT_FAILURE);
	}
	if (pthread_mutex_init(&stuff->time_protection, NULL))
	{
		pthread_mutex_destroy(&stuff->alive_protection);
		pthread_mutex_destroy(&stuff->eat_protection);
		clean_up(stuff);
		exit(EXIT_FAILURE);
	}
}

void	run_simulation(t_stuff *stuff)
{
	 pthread_t	*monitor;
	 pthread_t	*philo;

	gettimeofday(&stuff->tv_start, NULL);
	gettimeofday(&stuff->tv_beg, NULL);
	init_mutexs(stuff);
	 if (pthread_create(philo, NULL, start, stuff))
	 	exit(EXIT_FAILURE);
	if (pthread_create(philo, NULL, monitor, stuff))
		exit(EXIT_FAILURE);
}

void	init_philos(t_stuff *stuff)
{
	allocate_philos_forks(stuff);
	init_time(stuff);
	run_philos(stuff);
	if (stuff->p_pid != getpid())
		run_simulation(stuff);
	else
		wait_child(stuff);
}

void	init_stuff(t_stuff *stuff, int ac, char *av[])
{
	if (ac != 5 && ac != 6)
		return (write(2, "Invalid arguments!\n", 19), 1);
	stuff->alive = true;
	stuff->must_eat = 0;
	stuff->number_of_philos = ft_atoi(av[1]);
	stuff->t_to_die = ft_atoi(av[2]);
	stuff->t_to_eat = ft_atoi(av[3]);
	stuff->t_to_sleep = ft_atoi(av[4]);
	if (stuff->number_of_philos <= 0
		|| stuff->t_to_die <= 0
		|| stuff->t_to_eat <= 0
		|| stuff->t_to_sleep <= 0)
		return (write(2, "Invalid arguments!\n", 19), 1);
	if (ac == 6)
	{
		stuff->must_eat = ft_atoi(av[5]);
		if (stuff->must_eat <= 0)
			return (1);
	}
}

int main(int ac, char **av)
{
    t_stuff	stuff;
	int		reval;
	init_stuff(&stuff, ac, av);
	if (stuff.number_of_philos == 1)
		one_philo(stuff.t_to_die);
    init_philos(&stuff);
	return (0);
}
