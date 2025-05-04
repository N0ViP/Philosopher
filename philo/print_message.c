/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:52:55 by yjaafar           #+#    #+#             */
/*   Updated: 2025/04/23 21:52:56 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

static long long	time_ms(struct timeval *tv)
{
	return ((long long)tv->tv_sec * 1000LL + tv->tv_usec / 1000);
}

void    print_message(struct timeval *tv_start,
    struct timeval *tv, t_philo *philo, char *message)
{
    long long   tv_msec;
    tv_msec = time_ms(tv) - time_ms(tv_start);
    pthread_mutex_lock(&philo->stuff->print_protection);
    printf("%lld\t%i\t%s", tv_msec, philo->first_fork + 1, message);
    pthread_mutex_unlock(&philo->stuff->print_protection);
}
