/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 08:28:53 by yjaafar           #+#    #+#             */
/*   Updated: 2025/05/16 21:49:11 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	one_philo(t_stuff *stuff)
{
	pthread_mutex_t	fork;

	if (pthread_mutex_init(&fork, NULL))
		return ;
	printf("0\t1\tis thinking\n");
	pthread_mutex_lock(&fork);
	printf("0\t1\thas taken a fork\n");
	usleep(1000 * stuff->t_to_die);
	pthread_mutex_unlock(&fork);
	printf("0\t1\tdied\n");
}

bool	init_mutex(pthread_mutex_t *mtx[4])
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (pthread_mutex_init(mtx[i], NULL))
		{
			while (--i <= 0)
			{
				pthread_mutex_destroy(mtx[i]);
			}
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	init_each_philo(t_philo *philo, t_stuff *stuff, int i)
{
	pthread_mutex_t	*mtx[4];

	philo->stuff = stuff;
	philo->alive = 1;
	philo->eat = 0;
	philo->first_fork = (i);
	philo->second_fork = (i + 1) % (stuff->number_of_philos);
	philo->tv_beg = (struct timeval){0};
	mtx[0] = &stuff->forks[i];
	mtx[1] = &philo->eat_protection;
	mtx[2] = &philo->time_protection;
	mtx[3] = &philo->alive_protection;
	return (init_mutex(mtx));
}

static bool	allocate_stuff(t_stuff *stuff, t_philo **philos)
{
	stuff->philos = malloc(sizeof(pthread_t) * stuff->number_of_philos);
	if (!stuff->philos)
		return (false);
	stuff->forks = malloc(sizeof(pthread_mutex_t) * stuff->number_of_philos);
	if (!stuff->forks)
		return (free(stuff->philos), false);
	*philos = malloc(sizeof(t_philo) * stuff->number_of_philos);
	if (!*philos)
		return (free(stuff->philos), free(stuff->forks), false);
	if (pthread_mutex_init(&stuff->lock, NULL))
		return (free(stuff->philos), free(stuff->forks), free(philos), false);
	pthread_mutex_lock(&stuff->lock);
	return (true);
}

static bool	create_philos(t_philo *philos)
{
	t_stuff	*stuff;
	int		i;

	i = 0;
	stuff = philos->stuff;
	while (i < stuff->number_of_philos)
	{
		if (pthread_create(&stuff->philos[i], NULL, run_simulation, &philos[i]))
		{
			pthread_mutex_unlock(&stuff->lock);
			kill_philos(philos, i);
			free(philos);
			free(stuff->philos);
			free(stuff->forks);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	init_philo(t_philo *philos, t_stuff *stuff)
{
	int		i;

	i = 0;
	while (i < stuff->number_of_philos)
	{
		if (!init_each_philo(&philos[i], stuff, i))
		{
			pthread_mutex_unlock(&stuff->lock);
			destroy_mutex(philos, i);
			free(philos);
			free(stuff->philos);
			free(stuff->forks);
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	alloc_philos(t_stuff *stuff)
{
	t_philo			*philos;
	bool			reval;

	if (!allocate_stuff(stuff, &philos))
		return (false);
	gettimeofday(&stuff->tv_start, NULL);
	if (!init_philo(philos, stuff))
		return (false);
	if (!create_philos(philos))
		return (false);
	pthread_mutex_unlock(&stuff->lock);
	reval = monitoring(philos);
	free(philos);
	free(stuff->philos);
	free(stuff->forks);
	return (reval);
}

int	main(int ac, char *av[])
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
			return (0);
	}
	if (stuff.number_of_philos == 1)
		return (one_philo(&stuff), 0);
	reval = alloc_philos(&stuff);
	return (!reval);
}
