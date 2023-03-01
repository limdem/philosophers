/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 13:55:23 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/07 13:55:25 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	sleeping_thinking(t_philo *philo)
{
	if (print_action(philo, "is sleeping\n", 3))
		return (1);
	custom_usleep(philo->table, philo->sleep);
	return (0);
}

int	eating(t_philo *philo)
{
	sem_wait(philo->table->activity_sem[philo->id]);
	if (print_action(philo, "is eating\n", 2))
		return (1);
	philo->last_meal = get_curr_time();
	sem_post(philo->table->activity_sem[philo->id]);
	custom_usleep(philo->table, philo->eat);
	sem_post(philo->table->forks_sem);
	sem_post(philo->table->forks_sem);
	return (0);
}

int	forking(t_philo *philo)
{
	sem_wait(philo->table->forks_sem);
	if (print_action(philo, "has taken a fork\n", 1))
		return (1);
	if (!(philo->table->nbr > 1))
	{	
		usleep(philo->die * 1000 * 1.5);
		sem_post(philo->table->forks_sem);
		return (0);
	}
	sem_wait(philo->table->forks_sem);
	if (print_action(philo, "has taken a fork\n", 1))
		return (1);
	return (0);
}
