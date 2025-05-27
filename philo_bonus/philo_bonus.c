#include "philo_bonus.h"

int init_philos(t_stuff *stuff)
{
    t_philo			*philos;
	int				i;

	i = 0;
    stuff->philos = malloc(sizeof(pid_t) * stuff->number_of_philos);
	stuff->forks = sem_open("/forks", O_CREAT, 777, stuff->number_of_philos);
    philos = malloc(sizeof(t_philo) * stuff->number_of_philos);

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
			return (write(2, "Invalid arguments!\n", 19), 1);
	}
	if (stuff.number_of_philos == 1)
		return (one_philo(), 0);
    reval = init_philos(&stuff);
	return (reval);
}
