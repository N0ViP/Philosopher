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
	print_message(&philo->stuff->tv_start, &tv,
		philo->first_fork + 1, "is sleeping\n");
	ft_usleep(philo, philo->stuff->t_to_sleep);
}

void	take_forks(t_philo *philo)
{
	struct timeval	tv;

	if (philo->first_fork % 2)
	{
		gettimeofday(&tv, NULL);
		print_message(&philo->stuff->tv_start, &tv,
			philo->first_fork + 1, "has taken a fork\n");
		pthread_mutex_lock(&philo->stuff->forks[philo->first_fork]);
		gettimeofday(&tv, NULL);
		print_message(&philo->stuff->tv_start, &tv,
			philo->first_fork + 1, "has taken a fork\n");
		pthread_mutex_lock(&philo->stuff->forks[philo->second_fork]);
	}
	else
	{
		gettimeofday(&tv, NULL);
		print_message(&philo->stuff->tv_start, &tv,
			philo->first_fork + 1, "has taken a fork\n");
		pthread_mutex_lock(&philo->stuff->forks[philo->first_fork]);
		gettimeofday(&tv, NULL);
		print_message(&philo->stuff->tv_start, &tv,
			philo->first_fork + 1, "has taken a fork\n");
		pthread_mutex_lock(&philo->stuff->forks[philo->second_fork]);
	}
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->stuff->forks[philo->first_fork]);
	pthread_mutex_lock(&philo->stuff->forks[philo->second_fork]);
}

void	eating(t_philo *philo)
{
	int	alive;

	pthread_mutex_lock(&philo->alive_protection);
	alive = philo->alive;
	pthread_mutex_unlock(&philo->alive_protection);
	if (!alive)
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
		pthread_mutex_unlock(&philo->alive_protection);
		if (time >= 0)
			return ;
	}
}

void	*run_simulation(void *arg)
{
	t_philo			*philo;
	int				alive;

	philo = (t_philo *) arg;
	wait_4sec(philo);
	if (philo->first_fork + 1 % 2 == 0)
		ft_usleep(philo, 5);
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
