#include "philo_bonus.h"

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
		sem_close(stuff->lock);
		sem_close(stuff->forks);
		sem_unlink("/forks");
		sem_unlink("/lock");
		free(stuff->philos);
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
}

void	run_simulation(t_stuff *stuff)
{
	
}

bool	init_philos(t_stuff *stuff)
{
	allocate_philos_forks(stuff);
	init_time(stuff);
	run_philos(stuff);
	if (stuff->p_pid != getpid())
		run_simulation(stuff);
	else
		wait_child(stuff);
}

int main(int ac, char **av)
{
    t_stuff	stuff;
	int		reval;

	if (ac != 5 && ac != 6)
		return (write(2, "Invalid arguments!\n", 19), 1);
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
			return (1);
	}
	if (stuff.number_of_philos == 1)
		one_philo(stuff.t_to_die);
    reval = init_philos(&stuff);
	return (!reval);
}
