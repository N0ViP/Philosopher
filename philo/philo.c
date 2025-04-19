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

void	*start_sumilation(void *arg)
{
	int				tmp;
	struct timeval	tv;
	t_stuff			*stuff;

	stuff = (*t_stuff) arg;
	gettimeofday(&tv, NULL);
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
