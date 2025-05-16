/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 08:28:53 by yjaafar           #+#    #+#             */
/*   Updated: 2025/04/19 04:58:17 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

int	creat_monitor(t_philo *philos)
{
	pthread_t	monitor;
	long long	*reval;

	if (pthread_create(&monitor, NULL, monitoring, philos))
		return (1);
	if (pthread_join(monitor, (void *)&reval))
		return (1);
	return ((long long) (long long *)reval);
}

int	init_each_philo(t_philo *philo, t_stuff *stuff, int i)
{
	philo->stuff = stuff;
	philo->alive = 1;
	philo->eat = 0;
	philo->first_fork = (i);
	philo->second_fork = (i + 1) % (stuff->number_of_philos);
	philo->tv_beg = (struct timeval) {0};
	if (pthread_mutex_init(&stuff->forks[i], NULL))
		return (1);
	if (pthread_mutex_init(&philo->eat_protection, NULL))
		return (1);
	if (pthread_mutex_init(&philo->time_protection, NULL))
		return (1);
	if (pthread_mutex_init(&philo->alive_protection, NULL))
		return (1);
	return (0);
}

int	init_philos(t_stuff *stuff)
{
	t_philo			*philos;
	int				i;

	i = 0;
	philos = malloc(sizeof(t_philo) * stuff->number_of_philos);
	gettimeofday(&stuff->tv_start, NULL);
	while (i < stuff->number_of_philos)
	{
		if (init_each_philo(&philos[i], stuff, i))
			return (destroy_mutex(philos, i), free(philos), 1);
		i++;
	}
	i = 0;
	while (i < stuff->number_of_philos)
	{
		if (pthread_create(&stuff->philos[i], NULL, run_simulation, &philos[i]))
			return (kill_philos(philos, i), free(philos), 1);
		i++;
	}
	creat_monitor(philos);
	free(philos);
	return (0);
}

int main(int ac, char *av[])
{
	t_stuff	stuff;
	int		reval;

	if (ac != 5 && ac != 6)
		return (write(2, "Invalid arguments!\n", 19), 1);
	stuff.number_of_times_each_philo_must_eat = 0;
	stuff.number_of_philos = ft_atoi(av[1]);
	stuff.t_to_die = ft_atoi(av[2]);
	stuff.t_to_eat = ft_atoi(av[3]);
	stuff.t_to_sleep = ft_atoi(av[4]);
	if (stuff.t_to_die <= 0
		|| stuff.t_to_eat <= 0
		|| stuff.t_to_sleep <= 0)
		return (write(2, "Invalid arguments!\n", 19), 1);
	if (ac == 6)
	{
		stuff.number_of_times_each_philo_must_eat = ft_atoi(av[5]);
		if (stuff.number_of_times_each_philo_must_eat <= 0)
			return (write(2, "Invalid arguments!\n", 19), 1);
	}
	stuff.philos = malloc(sizeof(pthread_t) * stuff.number_of_philos);
	stuff.forks = malloc(sizeof(pthread_mutex_t) * stuff.number_of_philos);
	reval = init_philos(&stuff);
	return (free(stuff.philos),free(stuff.forks), reval);
}
