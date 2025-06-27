/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 08:29:06 by yjaafar           #+#    #+#             */
/*   Updated: 2025/05/16 21:50:55 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_stuff
{
	pthread_t		*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	struct timeval	tv_start;
	int				number_of_philos;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	int				must_eat;
}	t_stuff;

typedef struct t_philo
{
	pthread_mutex_t	eat_protection;
	pthread_mutex_t	alive_protection;
	pthread_mutex_t	time_protection;
	struct timeval	tv_beg;
	t_stuff			*stuff;
	int				first_fork;
	int				second_fork;
	int				eat;
	bool			alive;
}	t_philo;

int			ft_abs(int x);
int			ft_atoi(char *s);
void		*monitoring(void *arg);
bool		is_alive(t_philo *philo);
void		*run_simulation(void *arg);
void		take_forks(t_philo *philo);
long long	time_ms(struct timeval *tv);
void		put_fork(t_philo *philo, int fork);
void		take_fork(t_philo *philo, int fork);
void		ft_usleep(t_philo *philo, int time);
void		kill_philos(t_philo *philos, int n_of_philos);
void		destroy_mutex(t_philo *philos, int n_of_philos);
void		print_message(struct timeval *tv_start, struct timeval *tv,
				int id, char *message);

#endif
