/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 08:29:06 by yjaafar           #+#    #+#             */
/*   Updated: 2025/04/18 16:56:58 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

typedef	struct s_stuff
{
	pthread_t		*philos;
	pthread_mutex_t	*forks;
	int				number_of_philos;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	int				number_of_times_each_philo_must_eat;
}	t_stuff;

typedef struct t_philo
{
	t_stuff			*stuff;
	struct timeval	tv_beg;
	int				first_fork;
	int				second_fork;
	char			alive;
}	t_philo;

int		ft_atoi(char *s);
void	ft_exit(char err);
void    print_message(time_t tv_sec, int id, char *message);

#endif
