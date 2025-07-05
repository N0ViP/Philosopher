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
	exit (0);
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
		one_philo(stuff.t_to_die);
    reval = init_philos(&stuff);
	return (!reval);
}
