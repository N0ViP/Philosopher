/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:20:51 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/11 18:02:46 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	run_philos(t_stuff *stuff)
{
	int	i;

	i = 0;
	gettimeofday(&stuff->tv_start, NULL);
	while (i < stuff->number_of_philos)
	{
		if (getpid() != stuff->p_pid)
			return ;
		stuff->philo_id = i + 1;
		stuff->philos[i] = fork();
		if (stuff->philos[i] == -1)
		{
			kill_philos(stuff, i);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	sem_post(stuff->lock);
}

void	allocate_philos_forks(t_stuff *stuff)
{
	stuff->p_pid = getpid();
	stuff->philos = malloc (sizeof(pid_t) * stuff->number_of_philos);
	if (!stuff->philos)
	{
		exit (EXIT_FAILURE);
	}
	sem_unlink("/forks");
	sem_unlink("/lock");
	stuff->lock = sem_open("/lock", O_CREAT, 0777, 0);
	stuff->forks = sem_open("/forks", O_CREAT, 0777, stuff->number_of_philos);
	if (!stuff->forks || !stuff->lock)
	{
		clean_up(stuff);
		exit(EXIT_FAILURE);
	}
}

void	init_philos(t_stuff *stuff)
{
	allocate_philos_forks(stuff);
	run_philos(stuff);
	if (stuff->p_pid != getpid())
		run_simulation(stuff);
	else
		wait_child(stuff);
}
