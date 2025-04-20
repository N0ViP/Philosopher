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

void	thinking(t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	printf("%ll\t%i\tis thinking\n", tv.tv_usec, philo->philo_n);
}

void	eating(t_philo *philo)
{
	struct timeval	tv_before;
	struct timeval	tv_after;
	int				tmp;

	if (philo->philo_n == philo->stuff->number_of_philos - 1)
		 tmp = 0;
	else
		tmp = philo->philo_n + 1;
	gettimeofday(&tv_before, NULL);
	pthread_mutex_lock(&philo->stuff->forks[philo->philo_n]);
	gettimeofday(&tv_after, NULL);
	if (tv_after.tv_usec - tv_before.tv_usec >= philo->stuff->time_to_die)
		philo->is_died = 1;
	else
		printf("%ll\t%i\t has taken a fork\n", tv_after.tv_usec, philo->philo_n);
	gettimeofday(&tv_before, NULL);
	pthread_mutex_lock(&philo->stuff->forks[tmp]);
	gettimeofday(&tv_after, NULL);
	if (tv_after.tv_usec - tv_before.tv_usec >= philo->stuff->time_to_die)
		philo->is_died = 1;
	else
		printf("%ll\t%i\t has taken a fork\n", tv_after.tv_usec, philo->philo_n);
}

void	*start_sumilation(void *arg)
{
	struct timeval	tv_before;
	struct timeval	tv_after;
	t_philo			*philo;

	philo = (*t_philo) arg;
	while (ALIVE)
	{
		thinking(philo);
		eating(philo);
		sleeping(philo);
	}
}

void	init_sumilation(t_stuff *stuff)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = malloc(sizeof(t_philo *) * stuff->number_of_philos);
	while (i < stuff->number_of_philos)
	{
		philo[i].stuff = stuff;
		philo[i].philo_n = i;
		philo[i].is_died = 0;
		if (pthread_mutex_init(&(stuff->forks[i++]), NULL))
			return (distroy_mutex(stuff->forks, i - 1), 1);
	}
	i = 0;
	while (i < stuff->number_of_philos)
	{
		if (pthread_creat(&philo[i++], NULL, start_sumilation, stuff))
			return (distroy_mutex(stuff->forks, stuff->number_of_philos), 1);
	}
	i = 0;
	while (i < stuff->number_of_philos)
		if (pthread_join(stuff->philos[i++], NULL))
			return (distroy_mutex(stuff->forks, stuff->number_of_philos), 1);
	return (0);
}

int main(int ac, char **av)
{
	setbuf(stdout, NULL);
	t_stuff	stuff;

	if (ac != 5 || ac != 6)
	{
		ft_exit(2);
	}
	stuff.number_of_philos = ft_atoi(av[1]);
	stuff.time_to_die = ft_atoi(av[2]);
	stuff.time_to_eat = ft_atoi(av[3]);
	stuff.time_to_sleep = ft_atoi(av[4]);
	if (ac == 5)
		stuff.number_of_times_each_philo_must_eat = ft_atoi(av[5]);
	stuff.philos = (pthread_t *) malloc(sizeof(pthread_t)
		* stuff.number_of_philos);
	stuff.forks = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t)
		* stuff.number_of_philos);
	if (init_sumilation(stuff))
	{
		free(philos);
		exit(1);
	}
}
