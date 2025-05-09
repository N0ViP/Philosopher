# include "philo.h"

long long	time_ms(struct timeval *tv)
{
 	return ((long long)tv->tv_sec * 1000LL + tv->tv_usec / 1000);
}