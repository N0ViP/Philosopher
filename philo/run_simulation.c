#include "philo.h"

void	thinking(t_philo *philo)
{
	struct timeval	tv;
	int				alive;
	int				t_to_think;

	pthread_mutex_lock(&philo->alive_protection);
	alive = philo->alive;
	pthread_mutex_unlock(&philo->alive_protection);
	if (!alive)
		return ;
	if (philo->stuff->t_to_eat >= philo->stuff->t_to_sleep)
		t_to_think = philo->stuff->t_to_sleep;
	else
		t_to_think = philo->stuff->t_to_eat;
	gettimeofday(&tv, NULL);
	print_message(&philo->stuff->tv_start, &tv, philo->first_fork + 1, "is thinking\n");
	ft_usleep(philo, t_to_think);
}

void	sleeping(t_philo *philo)
{
	struct timeval	tv;
	int				alive;

	pthread_mutex_lock(&philo->alive_protection);
	alive = philo->alive;
	pthread_mutex_unlock(&philo->alive_protection);
	if (!alive)
		return ;
	gettimeofday(&tv, NULL);
	print_message(&philo->stuff->tv_start, &tv, philo->first_fork + 1, "is sleeping");
	ft_usleep(philo, philo->stuff->t_to_sleep);
}

void	eating(t_philo *philo)
{
	
}

void	*run_simulation(void *arg)
{
	t_philo			*philo;
	struct timeval	tv;
	long long		time;

	philo = (t_philo *) arg;
	while (true)
	{
		gettimeofday(&tv, NULL);
		pthread_mutex_lock(&philo->time_protection);
		time = time_ms(&tv) - time_ms(&philo->tv_beg);
		pthread_mutex_unlock(&philo->alive_protection);
		if (time >= 0)
			break ;
	}
	if (philo->first_fork + 1 % 2 == 0)
		ft_usleep(philo, 5);
	while (true)
	{
		thinking(philo);
		eating(philo);
		sleeping(philo);
	}
	return ((void *) 0);
}
