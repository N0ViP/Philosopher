#include "philo.h"

void	thinking(t_philo *philo)
{
	struct timeval	tv;
	int				alive;

	pthread_mutex_lock(&philo->alive_protection);
	alive = philo->alive;
	pthread_mutex_unlock(&philo->alive_protection);
	gettimeofday(&tv, NULL);
	if (!alive || time_ms(&tv) - time_ms(&philo->tv_beg) >= philo->stuff->t_to_die)
		return ;
	print_message(&philo->stuff->tv_start, &tv, philo->first_fork + 1, "is thinking\n");
}

void	sleeping(t_philo *philo)
{
	struct timeval	tv;
	int				alive;

	pthread_mutex_lock(&philo->alive_protection);
	alive = philo->alive;
	pthread_mutex_unlock(&philo->alive_protection);
	gettimeofday(&tv, NULL);
	print_message(&philo->stuff->tv_start, &tv,
		philo->first_fork + 1, "is sleeping\n");
	ft_usleep(philo, philo->stuff->t_to_sleep);
}

void	take_forks(t_philo *philo)
{
	struct timeval	tv;

	if (philo->first_fork % 2)
	{
		pthread_mutex_lock(&philo->stuff->forks[philo->first_fork]);
		gettimeofday(&tv, NULL);
		print_message(&philo->stuff->tv_start, &tv,
			philo->first_fork + 1, "has taken a fork\n");
		pthread_mutex_lock(&philo->stuff->forks[philo->second_fork]);
		gettimeofday(&tv, NULL);
		print_message(&philo->stuff->tv_start, &tv,
			philo->first_fork + 1, "has taken a fork\n");
	}
	else
	{
		pthread_mutex_lock(&philo->stuff->forks[philo->second_fork]);
		gettimeofday(&tv, NULL);
		print_message(&philo->stuff->tv_start, &tv,
			philo->first_fork + 1, "has taken a fork\n");
		pthread_mutex_lock(&philo->stuff->forks[philo->first_fork]);
		gettimeofday(&tv, NULL);
		print_message(&philo->stuff->tv_start, &tv,
			philo->first_fork + 1, "has taken a fork\n");
	}
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->stuff->forks[philo->first_fork]);
	pthread_mutex_unlock(&philo->stuff->forks[philo->second_fork]);
}

void	eating(t_philo *philo)
{
	int				alive;
	struct timeval	tv;

	pthread_mutex_lock(&philo->alive_protection);
	alive = philo->alive;
	pthread_mutex_unlock(&philo->alive_protection);
	gettimeofday(&tv, NULL);
	if (!alive || time_ms(&tv) - time_ms(&philo->tv_beg) >= philo->stuff->t_to_die)
		return ;
	take_forks(philo);
	pthread_mutex_lock(&philo->time_protection);
	gettimeofday(&philo->tv_beg, NULL);
	pthread_mutex_unlock(&philo->time_protection);
	print_message(&philo->stuff->tv_start, &philo->tv_beg,
		philo->first_fork + 1, "is eating\n");
	ft_usleep(philo, philo->stuff->t_to_eat);
	put_forks(philo);
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
	return ((void *) 0);
}
