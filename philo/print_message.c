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

int fill_buffer(long long tv_msec, int id, char *buffer)
{
    int tmp[25];
    int cnt;
    int i;

    cnt = 0;
    i = 0;
    while (tv_msec || i == 0)
    {
        tmp[i++] = (tv_msec % 10) + 48;
        tv_msec /= 10;
    }
    while (--i >= 0)
        buffer[cnt++] = tmp[i];
    i = 0;
    buffer[cnt++] = '\t';
    while (id || i == 0)
    {
        tmp[i++] = (id % 10) + 48;
        id /= 10;
    }
    while (--i >= 0)
        buffer[cnt++] = tmp[i];
    buffer[cnt++] = '\t';
    return (cnt);
}

void    print_message(struct timeval *tv_start,
    struct timeval *tv, int id, char *message)
{
    char        buffer[512];
    long long   tv_msec;
    int         cnt;
    int         i;

    i = 0;
    tv_msec = time_ms(tv) - time_ms(tv_start);
    cnt = fill_buffer(tv_msec, id, buffer);
    while (message[i])
    {
        buffer[cnt++] = message[i++];
    }
    write(1, buffer, cnt);
}
