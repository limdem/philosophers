/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_diner3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 10:40:10 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/08 10:41:33 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_continue_eat(t_philo *philo)
{
	philo->eat_count++;
	if (philo->eat_count == philo->table->must_eat)
	{
		pthread_mutex_lock(&philo->table->all_eat_mutex);
		philo->table->eat_count[philo->id] = 1;
		pthread_mutex_unlock(&philo->table->all_eat_mutex);
		return (1);
	}
	return (0);
}

void	next_check(int nbr, t_death_monitor *monitor)
{
	monitor->i++;
	usleep(100);
	if (monitor->i > nbr - 1)
	{
		monitor->i = 0;
		monitor->eat_counter = 0;
		usleep(500);
	}
}
