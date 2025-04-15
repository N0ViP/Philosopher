/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 08:28:53 by yjaafar           #+#    #+#             */
/*   Updated: 2025/04/15 03:53:51 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

void	start_sumilation(t_stuff *stuff)
{
	int				i;
	pthread_mutex_t *forks;
	pthread_t		*philos;

	i = 0;
	forks = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) 
		* stuff.number_of_philosophers);
	philos = (pthread_t *) malloc(sizeof(pthread_t)
		* stuff.number_of_philosophers);
	while (i < stuff.number_of_philosophers)
	{
		if (pthread_mutex_init(&forks[i++], NULL))
		{
			distroy_mutex(&forks, i - 1);
			exit(1)
		}
	}
	i = 0;
	while (i < stuff.number_of_philosophers)
	{
		pthread_creat(&);
	}
}

int main(int ac, char **av)
{
	t_stuff	stuff;

	if (ac != 5 || ac != 6)
	{
		ft_exit(2);
	}
	stuff.number_of_philosophers = ft_atoi(av[1]);
	stuff.time_to_die = ft_atoi(av[2]);
	stuff.time_to_eat = ft_atoi(av[3]);
	stuff.time_to_sleep = ft_atoi(av[4]);
	if (ac == 5)
		stuff.number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
	start_sumilation(&stuff);
}
