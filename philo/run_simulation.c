#include "philo.h"

void	thinking(t_philo *philo)
{
	struct timeval	tv;

	if (!is_alive(philo))
		return ;
	gettimeofday(&tv, NULL);
	print_message(&philo->stuff->tv_start, &tv, philo->first_fork + 1, "is thinking\n");
}

void	sleeping(t_philo *philo)
{
	struct timeval	tv;

	if (!is_alive(philo))
		return ;
	gettimeofday(&tv, NULL);
	print_message(&philo->stuff->tv_start, &tv,
		philo->first_fork + 1, "is sleeping\n");
	ft_usleep(philo, philo->stuff->t_to_sleep);
}

bool	take_fork(t_philo *philo, int fork)
{
	struct timeval	tv;

	pthread_mutex_lock(&philo->stuff->forks[fork]);
	gettimeofday(&tv, NULL);
	if (!is_alive(philo))
	{
		pthread_mutex_unlock(&philo->stuff->forks[fork]);
		return (false);
	}
	print_message(&philo->stuff->tv_start, &tv,
		philo->first_fork + 1, "has taken a fork\n");
	return (true);
}

void	put_fork(t_philo *philo, int fork)
{
	pthread_mutex_unlock(&philo->stuff->forks[fork]);
}

int	take_forks(t_philo *philo)
{
	if (philo->first_fork % 2)
	{
		if (!take_fork(philo, philo->first_fork))
			return (false);
		if (!take_fork(philo, philo->second_fork))
			return (put_fork(philo, philo->first_fork), false);
	}
	else
	{
		if (!take_fork(philo, philo->second_fork))
			return (false);
		if (!take_fork(philo, philo->first_fork))
			return (put_fork(philo, philo->second_fork), false);
	}
	return (true);
}

void	eating(t_philo *philo)
{
	if (!take_forks(philo))
		return ;
	pthread_mutex_lock(&philo->time_protection);
	gettimeofday(&philo->tv_beg, NULL);
	pthread_mutex_unlock(&philo->time_protection);
	if (!is_alive(philo))
	{
		put_fork(philo, philo->first_fork);
		put_fork(philo, philo->second_fork);
		return ;
	}
	print_message(&philo->stuff->tv_start, &philo->tv_beg,
		philo->first_fork + 1, "is eating\n");
	ft_usleep(philo, philo->stuff->t_to_eat);
	put_fork(philo, philo->first_fork);
	put_fork(philo, philo->second_fork);
}

void	wait_4sec(t_philo *philo)
{
	long long		time;
	struct timeval	tv;

	while (true)
	{
		gettimeofday(&tv, NULL);
		pthread_mutex_lock(&philo->time_protection);
		time = time_ms(&tv) - time_ms(&philo->tv_beg);
		pthread_mutex_unlock(&philo->time_protection);
		if (time >= 0)
			return ;
		usleep(500);
	}
}

void	*run_simulation(void *arg)
{
	t_philo			*philo;
	int				alive;

	philo = (t_philo *) arg;
	wait_4sec(philo);
	while (true)
	{
		pthread_mutex_lock(&philo->alive_protection);
		alive = philo->alive;
		pthread_mutex_unlock(&philo->alive_protection);
		if (!alive)
			break ;
		thinking(philo);
		eating(philo);
		sleeping(philo);
	}
	return (NULL);
}
