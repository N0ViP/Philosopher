#include "philo_bonus.h"

int	ft_atoi(char *s)
{
	long	res;

	if (!*s)
		return (-1);
	res = 0;
	while (*s >= 48 && *s <= 57)
	{
		res = (res << 3) + (res << 1) + (*s++ & 0X0f);
		if (res > INT_MAX)
			return (-1);
	}
	if (*s)
		return (-1);
	return ((int) res);
}

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
	sem_close(stuff->alive_protection);
	sem_close(stuff->time_protection);
	sem_close(stuff->eat_protection);
	sem_unlink(stuff->alive_protection_name);
	sem_unlink(stuff->time_protection_name);
	sem_unlink(stuff->eat_protection_name);
	free(stuff->alive_protection_name);
	free(stuff->time_protection_name);
	free(stuff->eat_protection_name);
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
		waitpid(pid, &ex_status, 0);
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
	stuff->lock = sem_open("/lock", O_CREAT, 0777, 0);
	stuff->forks = sem_open("/forks", O_CREAT, 0777, stuff->number_of_philos);
	if (!stuff->forks || !stuff->lock)
	{
		clean_up(stuff);
		exit(EXIT_FAILURE);
	}
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
			return ;
		stuff->philo_id = i + 1;
		stuff->philos[i] = fork();
		if (stuff->philos[i] == -1)
		{
			kill_philos(stuff, i);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	sem_post(stuff->lock);
}

void	philo_died(t_stuff *stuff, int pid)
{
	struct timeval	tv;
	int				i;

	i = 0;
	while (stuff->philos[i] != pid)
	{
		i++;
	}
	i++;
	kill_philos(stuff, stuff->number_of_philos);
	gettimeofday(&tv, NULL);
	gettimeofday(&stuff->tv_beg, NULL);
	printf("%lld\t%d\tdied\n", time_ms(&tv) - \
		time_ms(&stuff->tv_beg), i);
}

void	wait_child(t_stuff *stuff)
{
	int	child_pid;
	int	exit_child_val;
	int	i;

	i = 1;
	while (true)
	{
		child_pid = waitpid(-1, &exit_child_val, 0);
		if (exit_child_val != 0)
		{
			philo_died(stuff, child_pid);
			clean_up(stuff);
			exit(EXIT_FAILURE);
		}
		if (stuff->must_eat != 0 && i == stuff->number_of_philos)
		{
			kill_philos(stuff, stuff->number_of_philos);
			clean_up(stuff);
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
	printf("%lld\t%d\tis sleeping\n", time_ms(&tv) - \
		time_ms(&stuff->tv_start), stuff->philo_id);
	ft_usleep(stuff, stuff->t_to_sleep);
}

void	take_fork(t_stuff *stuff)
{
	struct timeval	tv;

	sem_wait(stuff->forks);
	if (!is_alive(stuff))
		return ;
	gettimeofday(&tv, NULL);
	printf("%lld\t%d\thas taken a fork\n", time_ms(&tv) - \
		time_ms(&stuff->tv_start), stuff->philo_id);
}

void	take_forks(t_stuff *stuff)
{
	take_fork(stuff);
	take_fork(stuff);
}

void	put_forks(t_stuff *stuff)
{
	sem_post(stuff->forks);
	sem_post(stuff->forks);
}

void	eating(t_stuff *stuff)
{
	take_forks(stuff);
	sem_wait(stuff->time_protection);
	gettimeofday(&stuff->tv_beg, NULL);
	sem_post(stuff->time_protection);
	if (!is_alive(stuff))
	{
		put_forks(stuff);
		return ;
	}
	printf("%lld\t%d\tis eating\n", time_ms(&stuff->tv_beg) - \
		time_ms(&stuff->tv_start), stuff->philo_id);
	ft_usleep(stuff, stuff->t_to_eat);
	put_forks(stuff);
	sem_wait(stuff->eat_protection);
	stuff->n_eat++;
	sem_post(stuff->eat_protection);
}

void	*start(void *arg)
{
	t_stuff	*stuff;

	stuff = (t_stuff *) arg;
	sem_wait(stuff->time_protection);
	gettimeofday(&stuff->tv_beg, NULL);
	sem_post(stuff->time_protection);
	if ((stuff->philo_id % 2))
		ft_usleep(stuff, stuff->t_to_eat);
	while (is_alive(stuff))
	{
		thinking(stuff);
		eating(stuff);
		sleeping(stuff);
	}
	return (NULL);
}

void	jon_philo(t_stuff *stuff, pthread_t philo)
{
	sem_wait(stuff->alive_protection);
	stuff->alive = false;
	sem_post(stuff->alive_protection);
	pthread_join(philo, NULL);
	clean_sems(stuff);
}

void	check_alive(t_stuff *stuff, pthread_t philo)
{
	struct timeval	tv;
	long long		time;

	sem_wait(stuff->time_protection);
	time = time_ms(&stuff->tv_beg);
	sem_post(stuff->time_protection);
	gettimeofday(&tv, NULL);
	if (time_ms(&tv) - time >= stuff->t_to_die)
	{
		jon_philo(stuff, philo);
		exit(EXIT_FAILURE);
	}
}

void	check_eat(t_stuff *stuff, pthread_t philo)
{
	int	n;

	sem_wait(stuff->eat_protection);
	n = stuff->n_eat;
	sem_post(stuff->eat_protection);
	if (stuff->must_eat && n >= stuff->must_eat)
	{
		jon_philo(stuff, philo);
		exit(EXIT_SUCCESS);
	}
}

void	monitor(t_stuff *stuff, pthread_t philo)
{
	while (true)
	{
		check_alive(stuff, philo);
		check_eat(stuff, philo);
	}
}

void	open_semaphores(t_stuff *stuff)
{
	sem_unlink(stuff->alive_protection_name);
	sem_unlink(stuff->time_protection_name);
	sem_unlink(stuff->eat_protection_name);
	stuff->alive_protection = sem_open(stuff->alive_protection_name, O_CREAT,  0777, 1);
	stuff->time_protection = sem_open(stuff->time_protection_name, O_CREAT,  0777, 1);
	stuff->eat_protection = sem_open(stuff->eat_protection_name, O_CREAT,  0777, 1);
	if (!stuff->alive_protection
		|| !stuff->time_protection
		|| !stuff->eat_protection)
	{
		clean_sems(stuff);
		exit(EXIT_FAILURE);
	}
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

int	ft_numlen(int n)
{
	int	i;

	i = 0;
	while (n)
	{
		i++;
		n /= 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;

	len = ft_numlen(n);
	res = malloc(len + 1);
	res[len--] = 0;
	while (n)
	{
		res[len--] = (n % 10) + 48;
		n /= 10;
	}
	return (res);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*res;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s2)
		return (NULL);
	res = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!res)
		return (free(s2), NULL);
	while (s1[i])
	{
		res[j++] = s1[i++];
	}
	i = 0;
	while (s2[i])
	{
		res[j++] = s2[i++];
	}
	res[j] = 0;
	return (free(s2), res);
}

void	init_semaphores(t_stuff *stuff)
{
	stuff->alive_protection_name = ft_strjoin("alive_protection_", ft_itoa(stuff->philo_id));
	stuff->time_protection_name = ft_strjoin("time_protection_", ft_itoa(stuff->philo_id));
	stuff->eat_protection_name = ft_strjoin("eat_protection_", ft_itoa(stuff->philo_id));
	if (!stuff->alive_protection_name
		|| !stuff->time_protection_name
		|| !stuff->eat_protection_name)
	{
		free (stuff->alive_protection_name);
		free (stuff->time_protection_name);
		free (stuff->eat_protection_name);
		clean_up(stuff);
		exit (EXIT_FAILURE);
	}
	open_semaphores(stuff);
}

void	run_simulation(t_stuff *stuff)
{
	pthread_t	philo;

	init_semaphores(stuff);
	sem_wait(stuff->lock);
	sem_post(stuff->lock);
	init_time(stuff);
	if (pthread_create(&philo, NULL, start, stuff))
	{
		clean_sems(stuff);
		exit(EXIT_FAILURE);
	}
	monitor(stuff, philo);
}

void	init_philos(t_stuff *stuff)
{
	allocate_philos_forks(stuff);
	run_philos(stuff);
	if (stuff->p_pid != getpid())
		run_simulation(stuff);
	else
		wait_child(stuff);
}

bool	init_stuff(t_stuff *stuff, int ac, char *av[])
{
	if (ac != 5 && ac != 6)
		return (write(2, "Invalid arguments!\n", 19), false);
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
		return (write(2, "Invalid arguments!\n", 19), false);
	if (ac == 6)
	{
		stuff->must_eat = ft_atoi(av[5]);
		if (stuff->must_eat <= 0)
			return (false);
	}
	return (true);
}

int main(int ac, char **av)
{
    t_stuff	stuff;

	stuff = (t_stuff) {0};
	if (!init_stuff(&stuff, ac, av))
		return (1);
	if (stuff.number_of_philos == 1)
		one_philo(stuff.t_to_die);
    init_philos(&stuff);
	return (0);
}
