#include "philo_bonus.h"

long long	time_ms(struct timeval *tv)
{
	return ((long long)(tv->tv_sec * 1000) + (tv->tv_usec / 1000));
}

int	ft_abs(int x)
{
	if (x < 0)
		return (-x);
	return (x);
}

bool	is_alive(t_stuff *stuff)
{
	bool	alive;
	sem_wait(stuff->alive_protection);
	alive = stuff->alive;
	sem_post(stuff->alive_protection);
	return (alive);
}

void	ft_usleep(t_stuff *stuff, int time)
{
	struct timeval	tv_before;
	struct timeval	tv_after;

	gettimeofday(&tv_before, NULL);
	while (true)
	{
		if (!is_alive(stuff))
			return ;
		gettimeofday(&tv_after, NULL);
		if (time_ms(&tv_after) - time_ms(&tv_before) >= time)
			break ;
		usleep(500);
	}
}

void	clean_up(t_stuff *stuff)
{
	sem_close(stuff->lock);
	sem_close(stuff->forks);
	sem_unlink("/forks");
	sem_unlink("/lock");
	free(stuff->philos);
}

void	clean_sems(t_stuff *stuff)
{
	sem_close(stuff->sem_alive_name);
	sem_close(stuff->sem_eat_name);
	sem_close(stuff->sem_time_name);
	sem_unlink(stuff->sem_alive_name);
	sem_unlink(stuff->sem_eat_name);
	sem_unlink(stuff->sem_time_name);
	free(stuff->sem_alive_name);
	free(stuff->sem_eat_name);
	free(stuff->sem_time_name);
	clean_up(stuff);
}

void	one_philo(int t_to_die)
{
	pid_t	pid;
	sem_t	*sem;
	int		ex_status;

	unlink("/forks");
	sem = sem_open("/forks", O_CREAT, 0777, 1);
	pid = fork();
	if (pid == 0)
	{
		printf("%d\t%d\tis thinking\n", 1, 0);
		sem_wait(sem);
		printf("%d\t%d\thas taken a fork\n", 1, 0);
		usleep(1000 * t_to_die);
		printf("%d\t%d\tdied\n", 1, t_to_die);
		sem_close(sem);
	}
	else
	{
		waitpid(pid, ex_status, 0);
		sem_close(sem);
		unlink("/forks");
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
		stuff->philo_id = i + 1;
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

void	thinking(t_stuff *stuff)
{
	struct timeval	tv;
	int				time;
	int				t_to_eat_sleep;

	if (!is_alive(stuff))
		return ;
	gettimeofday(&tv, NULL);
	printf("%lld\t%d\tis thinking\n", time_ms(&tv) - \
		time_ms(&stuff->tv_start), stuff->philo_id);
	time = ft_abs(stuff->t_to_eat - stuff->t_to_sleep) + 10;
	t_to_eat_sleep = stuff->t_to_eat + stuff->t_to_sleep;
	if (t_to_eat_sleep < stuff->t_to_die)
	{
		while (time + t_to_eat_sleep >= stuff->t_to_die)
			time /= 2;
	}
	ft_usleep(stuff, time);
}

void	sleeping(t_stuff *stuff)
{
	struct timeval	tv;

	if (!is_alive(stuff))
		return ;
	gettimeofday(&tv, NULL);
	printf("%lld\t%d\tis slepping\n", time_ms(&tv) - \
		time_ms(&stuff->tv_start), stuff->philo_id);
	ft_usleep(stuff, stuff->t_to_sleep);
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
///???????
void	monitor(t_stuff *stuff)
{
	;
}

void	open_semaphores(t_stuff *stuff)
{
	sem_unlink(stuff->sem_alive_name);
	sem_unlink(stuff->sem_eat_name);
	sem_unlink(stuff->sem_time_name);
	stuff->alive_protection = sem_open(stuff->sem_alive_name, O_CREAT,  0777, 1);
	stuff->eat_protection = sem_open(stuff->sem_eat_name, O_CREAT,  0777, 1);
	stuff->time_protection = sem_open(stuff->sem_time_name, O_CREAT,  0777, 1);
	if (!stuff->alive_protection
		|| !stuff->eat_protection
		|| !stuff->time_protection)
	{
		clean_sems(stuff);
		exit(EXIT_FAILURE);
	}
}

void	init_semaphores(t_stuff *stuff)
{
	stuff->sem_alive_name = ft_strjoin("alive_", ft_itoa(stuff->philo_id));
	stuff->sem_eat_name = ft_strjoin("eat_", ft_itoa(stuff->philo_id));
	stuff->sem_time_name = ft_strjoin("time_", ft_itoa(stuff->philo_id));
	if (!stuff->sem_alive_name
		|| !stuff->sem_eat_name
		|| !stuff->sem_time_name)
	{
		free(stuff->sem_alive_name);
		free(stuff->sem_eat_name);
		free(stuff->sem_time_name);
		clean_up(stuff);
		exit (EXIT_FAILURE);
	}
	open_semaphores(stuff);
}

void	run_simulation(t_stuff *stuff)
{
	 pthread_t	*philo;

	gettimeofday(&stuff->tv_start, NULL);
	gettimeofday(&stuff->tv_beg, NULL);
	init_semaphores(stuff);
	if (pthread_create(philo, NULL, start, stuff))
	{
		clean_sems(stuff);
		exit(EXIT_FAILURE);
	}
	monitor(stuff);
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
