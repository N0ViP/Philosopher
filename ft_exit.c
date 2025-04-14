/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 09:48:44 by yjaafar           #+#    #+#             */
/*   Updated: 2025/04/14 10:37:19 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_exit(char err)
{
	if (err == 2)
		printf("Usage: ./philo <number_of_philosophers> <time_to_die> <time_to_eat>\
	<time_to_sleep> <number_of_times_each_philosopher_must_eat(optinal)>");
	else if (err == 1)
		printf("Invalid argument\n");
	exit(err);
}
