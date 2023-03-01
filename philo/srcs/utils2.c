/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 02:29:40 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/05 02:32:11 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

unsigned long long	get_curr_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	custom_usleep(t_table *table, unsigned long long time_sleep)
{
	unsigned long long	curr_time;
	int					dead;

	curr_time = get_curr_time();
	while (1)
	{
		pthread_mutex_lock(&table->death);
		dead = table->dead;
		pthread_mutex_unlock(&table->death);
		if (get_curr_time() - curr_time >= time_sleep || dead)
			break ;
		usleep(400);
	}
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}
