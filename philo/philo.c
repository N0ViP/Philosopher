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

void	init_philo(t_philo *philo, t_stuff *stuff, int i)
{
	philo->stuff = stuff;
	philo->first_fork = i;
	philo->second_fork = (i + 1) * (i != stuff->number_of_philos - 1);
	philo->alive = 1;
}

int	init_sumilation(t_stuff *stuff)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = malloc(sizeof(t_philo) * stuff->number_of_philos);
	while (i < stuff->number_of_philos)
	{
		init_philo(&philos[i], stuff, i);
		if (pthread_mutex_init(&philos[i++].stuff->forks, NULL))
			return (free(philos), 1);
	}
	i = 0;
	while (i < stuff->number_of_philos)
		if (pthread_create(philos[i].))
}

int main(int ac, char *av[])
{
	t_stuff	stuff;

	if (ac != 5 && ac != 6)
		exit(1);
	stuff.number_of_philos = ft_atoi(av[1]);	
	stuff.t_to_die = ft_atoi(av[2]);	
	stuff.t_to_eat = ft_atoi(av[3]);	
	stuff.t_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		stuff.number_of_times_each_philo_must_eat = ft_atoi(av[5]);
	stuff.philos = malloc(sizeof(pthread_t) * stuff.number_of_philos);
	stuff.forks = malloc(sizeof(pthread_mutex_t) * stuff.number_of_philos);
	init_sumilation(&stuff);
}