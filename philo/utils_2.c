/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:55:12 by yjaafar           #+#    #+#             */
/*   Updated: 2025/05/16 21:55:22 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(t_philo *philo, int time)
{
	struct timeval	tv_before;
	struct timeval	tv_after;

	gettimeofday(&tv_before, NULL);
	while (true)
	{
		if (!is_alive(philo))
			return ;
		gettimeofday(&tv_after, NULL);
		if (time_ms(&tv_after) - time_ms(&tv_before) >= time)
			break ;
		usleep(500);
	}
}

void	take_fork(t_philo *philo, int fork)
{
	struct timeval	tv;

	pthread_mutex_lock(&philo->stuff->forks[fork]);
	gettimeofday(&tv, NULL);
	if (!is_alive(philo))
		return ;
	printf("%lld\t%d\thas taken a fork\n", time_ms(&tv) - \
		time_ms(&philo->stuff->tv_start), philo->first_fork + 1);
}

void	put_fork(t_philo *philo, int fork)
{
	pthread_mutex_unlock(&philo->stuff->forks[fork]);
}

void	take_forks(t_philo *philo)
{
	if (philo->first_fork % 2)
	{
		take_fork(philo, philo->first_fork);
		take_fork(philo, philo->second_fork);
	}
	else
	{
		take_fork(philo, philo->second_fork);
		take_fork(philo, philo->first_fork);
	}
}
