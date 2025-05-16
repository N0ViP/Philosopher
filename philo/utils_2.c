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

bool	take_fork(t_philo *philo, int fork)
{
	struct timeval	tv;

	pthread_mutex_lock(&philo->stuff->forks[fork]);
	gettimeofday(&tv, NULL);
	if (!is_alive(philo))
	{
		pthread_mutex_unlock(&philo->stuff->forks[fork]);
		return (false);
	}
	print_message(&philo->stuff->tv_start, &tv,
		philo->first_fork + 1, "has taken a fork\n");
	return (true);
}

void	put_fork(t_philo *philo, int fork)
{
	pthread_mutex_unlock(&philo->stuff->forks[fork]);
}

int	take_forks(t_philo *philo)
{
	if (philo->first_fork % 2)
	{
		if (!take_fork(philo, philo->first_fork))
			return (false);
		if (!take_fork(philo, philo->second_fork))
			return (put_fork(philo, philo->first_fork), false);
	}
	else
	{
		if (!take_fork(philo, philo->second_fork))
			return (false);
		if (!take_fork(philo, philo->first_fork))
			return (put_fork(philo, philo->second_fork), false);
	}
	return (true);
}
