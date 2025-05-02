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

long long	time_ms(struct timeval *tv)
{
	return ((long long)tv->tv_sec * 1000LL + tv->tv_usec / 1000);
}

void	thinking(t_philo *philo)
{
	struct timeval	tv_before;
	struct timeval	tv_after;

	gettimeofday(&tv_before, NULL);
	print_message(&tv_before, philo->first_fork + 1, "is thinking\n");
	while (1)
	{
		gettimeofday(&tv_after, NULL);
		if (time_ms(&tv_after) - time_ms(&tv_before) >= 1)
			break;
	}
}

void	take_forks(t_philo *philo)
{
	struct timeval	tv;
	if (philo->first_fork % 2 == 0)
	{
		pthread_mutex_lock(&philo->stuff->forks[philo->first_fork]);
		gettimeofday(&tv, NULL);
		print_message(&tv, philo->first_fork + 1, "has taken a fork\n");
		pthread_mutex_lock(&philo->stuff->forks[philo->second_fork]);
		gettimeofday(&tv, NULL);
		print_message(&tv, philo->first_fork + 1, "has taken a fork\n");
	}
	else
	{
		pthread_mutex_lock(&philo->stuff->forks[philo->second_fork]);
		gettimeofday(&tv, NULL);
		print_message(&tv, philo->first_fork + 1, "has taken a fork\n");
		pthread_mutex_lock(&philo->stuff->forks[philo->first_fork]);
		gettimeofday(&tv, NULL);
		print_message(&tv, philo->first_fork + 1, "has taken a fork\n");
	}
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->stuff->forks[philo->first_fork]);
	pthread_mutex_unlock(&philo->stuff->forks[philo->second_fork]);
}

void	eating(t_philo *philo)
{
	struct timeval	tv_after;
	
	take_forks(philo);
	pthread_mutex_lock(&philo->time_protection);
	gettimeofday(&philo->tv_beg, NULL);
	pthread_mutex_unlock(&philo->time_protection);
	print_message(&philo->tv_beg, philo->first_fork + 1, "is eating\n");
	while (1)
	{
		gettimeofday(&tv_after, NULL);
		if (time_ms(&tv_after) - time_ms(&philo->tv_beg) >= philo->stuff->t_to_eat)
			break;
	}
	pthread_mutex_lock(&philo->eat_protection);
	philo->eat++;
	pthread_mutex_unlock(&philo->eat_protection);
	put_forks(philo);
}

void	sleeping(t_philo *philo)
{
	struct timeval	tv_before;
	struct timeval	tv_after;

	gettimeofday(&tv_before, NULL);
	print_message(&tv_before, philo->first_fork + 1, "is sleeping\n");
	while (1)
	{
		gettimeofday(&tv_after, NULL);
		if (time_ms(&tv_after) - time_ms(&philo->tv_beg) >= philo->stuff->t_to_sleep)
			break;
	}
}

void	*start_sumilation(void *arg)
{
	t_philo			*philo;
	struct timeval	tv;

	philo = (t_philo *) arg;
	while (1)
	{
		gettimeofday(&tv, NULL);
		if (time_ms(&tv) - time_ms(&philo->tv_beg) >= 0)
			break;
	}
	while (philo->alive)
	{
		thinking(philo);
		eating(philo);
		sleeping(philo);
	}
	return ((void *)0);
}

void	init_philo(t_philo *philo, t_stuff *stuff, struct timeval *tv, int i)
{
	pthread_mutex_init(&philo->time_protection, NULL);
	philo->stuff = stuff;
	philo->eat = 0;
	philo->first_fork = i;
	philo->second_fork = (i + 1) % philo->stuff->number_of_philos;
	philo->alive = 1;
	philo->tv_beg = (struct timeval) {tv->tv_sec + 4, tv->tv_usec};
}

void	loop_1(t_philo *philos)
{
	int				i;
	struct timeval	tv;
	int				number_of_philos;

	number_of_philos = philos[0].stuff->number_of_philos;
	while (1)
	{
		i = 0;
		while (i < number_of_philos)
		{
			pthread_mutex_lock(&philos[i].time_protection);
			gettimeofday(&tv, NULL);
			if (time_ms(&tv) - time_ms(&philos[i].tv_beg) >= philos[i].stuff->t_to_die)
			{
				print_message(&tv, philos[i].first_fork + 1, "is died\n");
				exit(1);
			}
			pthread_mutex_unlock(&philos[i].time_protection);
			i++;
		}
	}
}
void	loop_2(t_philo *philos)
{
	int				i;
	struct timeval	tv;
	int				number_of_philos;

	number_of_philos = philos[0].stuff->number_of_philos;
	while (1)
	{
		i = 0;
		while (i < number_of_philos)
		{
			pthread_mutex_lock(&philos[i].time_protection);
			gettimeofday(&tv, NULL);
			if (time_ms(&tv) - time_ms(&philos[i].tv_beg) >= philos[i].stuff->t_to_die)
			{
				print_message(&tv, philos[i].first_fork + 1, "is died\n");
				exit(1);
			}
			pthread_mutex_unlock(&philos[i].time_protection);
			i++;
		}
	}
}

void	*monitoring(void *arg)
{
	t_philo			*philos;

	philos = (t_philo *) arg;
	if (!philos[0].stuff->number_of_times_each_philo_must_eat)
		loop_1(philos);
	else
		loop_2(philos);
	return ((void *)0);
}

int	init_sumilation(t_stuff *stuff)
{
	struct timeval	tv;
	pthread_t		monitorn_id;
	t_philo			*philos;
	int				i;

	i = 0;
	philos = malloc(sizeof(t_philo) * stuff->number_of_philos);
	gettimeofday(&tv, NULL);
	while (i < stuff->number_of_philos)
	{
		init_philo(&philos[i], stuff, &tv, i);
		if (pthread_mutex_init(&philos[i].stuff->forks[i], NULL))
			return (free(philos), 1);
		i++;
	}
	i = 0;
	while (i < stuff->number_of_philos)
	{
		if (pthread_create(&stuff->philos[i], NULL, start_sumilation, &philos[i]))
			return (free(philos), 1);
		i++;
	}
	if (pthread_create(&monitorn_id, NULL, monitoring, philos))
			return (free(philos), 1);
	i = 0;
	while (i < stuff->number_of_philos)
		if (pthread_join(stuff->philos[i++], NULL))
			return (free(philos), 1);
	free (philos);
	return (0);
}

int main(int ac, char *av[])
{
	t_stuff	stuff;

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
	init_sumilation(&stuff);
	free (stuff.philos);
	free (stuff.forks);
	return (0);
}
