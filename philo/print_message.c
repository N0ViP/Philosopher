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

int fill_buffer(long long tv_msec, int id, char *buffer)
{
    int cnt;
    int tmp[25];
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

void    print_message(time_t tv_sec, int id, char *message)
{
    char        buffer[512];
    long long   tv_msec;
    int         cnt;
    int         i;

    i = 0;
    tv_msec = tv_sec * 100;
    cnt = fill_buffer(tv_msec, id, buffer);
    while (message[i])
    {
        buffer[cnt++] = message[i++];
    }
    write(1, buffer, cnt);
}
