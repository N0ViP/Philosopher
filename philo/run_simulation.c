/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:53:36 by yjaafar           #+#    #+#             */
/*   Updated: 2025/05/16 21:54:52 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking(t_philo *philo)
{
	struct timeval	tv;
	int				time;
	int				t_to_eat_sleep;

	if (!is_alive(philo))
		return ;
	gettimeofday(&tv, NULL);
	print_message(&philo->stuff->tv_start, &tv,
		philo->first_fork + 1, "is thinking\n");
	time = ft_abs(philo->stuff->t_to_eat - philo->stuff->t_to_sleep) + 10;
	t_to_eat_sleep = philo->stuff->t_to_eat + philo->stuff->t_to_sleep;
	if (t_to_eat_sleep < philo->stuff->t_to_die)
	{
		while (time + t_to_eat_sleep >= philo->stuff->t_to_die)
			time /= 2;
	}
	ft_usleep(philo, time);
}

void	sleeping(t_philo *philo)
{
	struct timeval	tv;

	if (!is_alive(philo))
		return ;
	gettimeofday(&tv, NULL);
	print_message(&philo->stuff->tv_start, &tv,
		philo->first_fork + 1, "is sleeping\n");
	ft_usleep(philo, philo->stuff->t_to_sleep);
}

void	eating(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->time_protection);
	gettimeofday(&philo->tv_beg, NULL);
	pthread_mutex_unlock(&philo->time_protection);
	if (!is_alive(philo))
	{
		put_fork(philo, philo->first_fork);
		put_fork(philo, philo->second_fork);
		return ;
	}
	print_message(&philo->stuff->tv_start, &philo->tv_beg,
		philo->first_fork + 1, "is eating\n");
	ft_usleep(philo, philo->stuff->t_to_eat);
	put_fork(philo, philo->first_fork);
	put_fork(philo, philo->second_fork);
	pthread_mutex_lock(&philo->eat_protection);
	philo->eat++;
	pthread_mutex_unlock(&philo->eat_protection);
}

void	*run_simulation(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *) arg;
	pthread_mutex_lock(&philo->stuff->lock);
	pthread_mutex_unlock(&philo->stuff->lock);
	pthread_mutex_lock(&philo->time_protection);
	gettimeofday(&philo->tv_beg, NULL);
	pthread_mutex_unlock(&philo->time_protection);
	if (!(philo->first_fork % 2))
		ft_usleep(philo, philo->stuff->t_to_eat);
	while (is_alive(philo))
	{
		thinking(philo);
		eating(philo);
		sleeping(philo);
	}
	return (NULL);
}
