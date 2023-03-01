/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_diner2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 01:57:14 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/05 02:03:18 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	end_diner(t_philo *philo, t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nbr)
	{
		pthread_join((philo + i)->t, NULL);
		i++;
	}
	i = 0;
	while (i < table->nbr)
	{
		pthread_mutex_destroy(table->m_forks + i);
		pthread_mutex_destroy(table->activity + i);
		free((philo + i)->id_print);
		i++;
	}
	pthread_mutex_destroy(&(table->death));
	free(table->m_forks);
	free(table->activity);
	free(table->last_meals);
	free(table->eat_count);
	free(philo);
}

void	prepare_forks(int id, t_forks *forks, int nbr)
{
	if (id)
		forks->r_fork = id - 1;
	else
		forks->r_fork = nbr - 1;
	forks->l_fork = id;
	if (id % 2)
	{
		forks->f_fork = forks->r_fork;
		forks->s_fork = forks->l_fork;
	}
	else
	{
		forks->f_fork = forks->l_fork;
		forks->s_fork = forks->r_fork;
	}
}

void	routine_starter(t_philo *philo)
{
	pthread_mutex_lock(philo->table->activity + philo->id);
	philo->table->last_meals[philo->id] = philo->table->start;
	pthread_mutex_unlock(philo->table->activity + philo->id);
	prepare_forks(philo->id, &philo->forks, philo->table->nbr);
	diner_starter(philo->table);
	if (philo->id % 2 != 0)
		usleep(philo->eat * 1000 / 2);
}

int	check_continue_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->death);
	if (philo->table->dead)
	{
		pthread_mutex_unlock(&philo->table->death);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->death);
	return (0);
}

void	diner_starter(t_table *table)
{
	unsigned long long	delay;

	delay = table->start - get_curr_time();
	custom_usleep(table, delay);
}
