/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 08:28:53 by yjaafar           #+#    #+#             */
/*   Updated: 2025/04/14 10:32:02 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

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
}
