/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_diner3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 10:53:53 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/08 10:54:20 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_continue_eat(t_philo *philo)
{
	philo->eat_count++;
	if (philo->eat_count == philo->must_eat)
	{
		sem_wait(philo->table->stop_sem);
		philo->table->stop = 1;
		sem_post(philo->table->stop_sem);
		sem_post(philo->table->gen_sem[philo->id]);
		return (1);
	}
	return (0);
}
