/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:21:23 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/11 16:21:24 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	thinking(t_stuff *stuff)
{
	struct timeval	tv;
	int				time;
	int				t_to_eat_sleep;

	if (!is_alive(stuff))
		return ;
	gettimeofday(&tv, NULL);
	printf("%lld\t%d\tis thinking\n", time_ms(&tv) - \
		time_ms(&stuff->tv_start), stuff->philo_id);
	time = ft_abs(stuff->t_to_eat - stuff->t_to_sleep) + 10;
	t_to_eat_sleep = stuff->t_to_eat + stuff->t_to_sleep;
	if (t_to_eat_sleep < stuff->t_to_die)
	{
		while (time + t_to_eat_sleep >= stuff->t_to_die)
			time /= 2;
	}
	usleep(time * 1000);
}

void	sleeping(t_stuff *stuff)
{
	struct timeval	tv;

	if (!is_alive(stuff))
		return ;
	gettimeofday(&tv, NULL);
	printf("%lld\t%d\tis sleeping\n", time_ms(&tv) - \
		time_ms(&stuff->tv_start), stuff->philo_id);
	usleep(stuff->t_to_sleep * 1000);
}

void	eating(t_stuff *stuff)
{
	take_forks(stuff);
	if (!is_alive(stuff))
	{
		put_forks(stuff);
		return ;
	}
	sem_wait(stuff->time_protection);
	gettimeofday(&stuff->tv_beg, NULL);
	sem_post(stuff->time_protection);
	printf("%lld\t%d\tis eating\n", time_ms(&stuff->tv_beg) - \
		time_ms(&stuff->tv_start), stuff->philo_id);
	usleep(stuff->t_to_eat * 1000);
	put_forks(stuff);
	sem_wait(stuff->eat_protection);
	stuff->n_eat++;
	sem_post(stuff->eat_protection);
}

void	*start(void *arg)
{
	t_stuff	*stuff;

	stuff = (t_stuff *) arg;
	sem_wait(stuff->time_protection);
	gettimeofday(&stuff->tv_beg, NULL);
	sem_post(stuff->time_protection);
	if (!(stuff->philo_id % 2))
		usleep(stuff->t_to_eat * 1000);
	while (is_alive(stuff))
	{
		thinking(stuff);
		eating(stuff);
		sleeping(stuff);
	}
	return (NULL);
}

void	run_simulation(t_stuff *stuff)
{
	pthread_t	philo;

	init_semaphores(stuff);
	sem_wait(stuff->lock);
	sem_post(stuff->lock);
	init_time(stuff);
	if (pthread_create(&philo, NULL, start, stuff))
	{
		clean_sems(stuff);
		exit(EXIT_FAILURE);
	}
	monitor(stuff, philo);
}
