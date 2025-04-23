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

void	kill_philos(t_philo *philos)
{
	int	i;
	int	n;

	i = 0;
	n = philos[i].stuff->number_of_philos;
	while (i < n)
	{
		philos[i].alive = 0;
		i++;
	}
}

void	*monitoring(t_philo *philos)
{
	struct timeval	tv;
	int				i;

	i = 0;
	while (1)
	{
		while (i < philos->stuff->number_of_philos)
		{
			gettimeofday(&tv, NULL);
			if (philos[i].tv_beg.tv_sec != 0 && tv.tv_usec - philos[i].tv_beg.tv_usec >= philos[i].stuff->t_to_die)
			{
				printf("%ld\t%i\tdied\n", tv.tv_usec, i + 1);
				return (kill_philos(philos), (void *)0);
			}
			i++;
		}
	}
}

void	thinking(t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	printf("%ld\t%i\tis thinking\n", tv.tv_usec, philo->first_fork + 1);
}

void	eating(t_philo *philo)
{
	struct timeval	tv_before;

	pthread_mutex_lock(&philo->stuff->forks[philo->first_fork]);
	gettimeofday(&tv_before, NULL);
	printf("%ld\t%i\thas taken a fork\n", tv_before.tv_usec, philo->first_fork + 1);
	pthread_mutex_lock(&philo->stuff->forks[philo->second_fork]);
	gettimeofday(&tv_before, NULL);
	printf("%ld\t%i\thas taken a fork\n", tv_before.tv_usec, philo->first_fork + 1);
	gettimeofday(&philo->tv_beg, NULL);
	printf("%ld\t%i\tis eating\n", philo->tv_beg.tv_usec, philo->first_fork + 1);
	usleep(philo->stuff->t_to_eat * 1000);
	pthread_mutex_unlock(&philo->stuff->forks[philo->first_fork]);
	pthread_mutex_unlock(&philo->stuff->forks[philo->second_fork]);
}

void	sleeping(t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	printf("%ld\t%i\tis sleeping\n", tv.tv_usec, philo->first_fork + 1);
	usleep(philo->stuff->t_to_sleep * 1000);
}

void	*start_sumilation(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo->alive)
			thinking(philo);
		if (philo->alive)
			eating(philo);
		if (philo->alive)
			sleeping(philo);
	}
	return ((void*)0);
}

int	init_sumilation(t_stuff *stuff)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = malloc(sizeof(t_philo) * stuff->number_of_philos);
	while (i < stuff->number_of_philos)
	{
		philo[i].stuff = stuff;
		philo[i].first_fork = i;
		philo[i].second_fork = (i + 1) * (i == philo[i].stuff->number_of_philos - 1);
		philo[i].tv_beg = (struct timeval) {0};
		philo[i].alive = 1;
		if (pthread_mutex_init(&(stuff->forks[i++]), NULL))
			return (1);
	}
	i = 0;
	while (i < stuff->number_of_philos)
	{
		if (pthread_create(&philo[i].stuff->philos[i], NULL, start_sumilation, &philo[i]))
			return (1);
		i++;
	}
	monitoring(philo);
	i = 0;
	while (i < stuff->number_of_philos)
		if (pthread_join(stuff->philos[i++], NULL))
			return (1);
	return (0);
}

int main(int ac, char **av)
{
	setbuf(stdout, NULL);
	t_stuff	stuff;

	if (ac != 5 && ac != 6)
	{
		ft_exit(2);
	}
	stuff.number_of_philos = ft_atoi(av[1]);
	stuff.t_to_die = ft_atoi(av[2]);
	stuff.t_to_eat = ft_atoi(av[3]);
	stuff.t_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		stuff.number_of_times_each_philo_must_eat = ft_atoi(av[5]);
	stuff.philos = (pthread_t *) malloc(sizeof(pthread_t)
		* stuff.number_of_philos);
	stuff.forks = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t)
		* stuff.number_of_philos);
	if (init_sumilation(&stuff))
	{
		exit(1);
	}
}
