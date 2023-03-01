/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_diner2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 13:56:27 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/07 13:56:59 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	handle_sem_array(t_philo *philo, int end, t_prep_end prep_end)
{
	while (++prep_end.index < philo->table->nbr)
	{
		sem_close(philo->table->gen_sem[prep_end.index]);
		sem_close(philo->table->activity_sem[prep_end.index]);
		if (end)
		{
			prep_end.tmp = ft_itoa(prep_end.index);
			if (!prep_end.tmp)
				return (1);
			prep_end.name = ft_strjoin("/gen_sem", prep_end.tmp);
			if (!prep_end.name)
				return (1);
			sem_unlink(prep_end.name);
			free(prep_end.name);
			prep_end.name = ft_strjoin("/activity_sem", prep_end.tmp);
			if (!prep_end.name)
				return (1);
			free(prep_end.tmp);
			sem_unlink(prep_end.name);
			free(prep_end.name);
		}
	}
	return (0);
}

int	end_diner(t_philo *philo, int end)
{
	t_prep_end	prep_end;

	prep_end.index = -1;
	if (handle_sem_array(philo, end, prep_end))
		return (1);
	free(philo->table->gen_sem);
	free(philo->table->activity_sem);
	sem_close(philo->table->forks_sem);
	if (end)
		sem_unlink(FORKS_SEM);
	sem_close(philo->table->stop_sem);
	if (end)
		sem_unlink(STOP_SEM);
	sem_close(philo->table->print_sem);
	if (end)
		sem_unlink(PRINT_SEM);
	return (0);
}

int	check_continue_death(t_table *table)
{
	sem_wait(table->stop_sem);
	if (table->stop)
	{
		sem_post(table->stop_sem);
		return (1);
	}
	sem_post(table->stop_sem);
	return (0);
}

void	diner_starter(t_table *table)
{
	unsigned long long	delay;

	delay = table->start - get_curr_time();
	custom_usleep(table, delay);
}

void	*death_monitoring(void *arg)
{
	t_philo	*philo;

	philo = arg;
	diner_starter(philo->table);
	while (1)
	{
		sem_wait(philo->table->stop_sem);
		if (philo->table->stop)
		{
			sem_post(philo->table->stop_sem);
			return (NULL);
		}
		sem_post(philo->table->stop_sem);
		sem_wait(philo->table->activity_sem[philo->id]);
		if (get_curr_time() - philo->last_meal > philo->die)
		{
			if (print_action(philo, "has died\n", 0))
				return (NULL);
			sem_post(philo->table->activity_sem[philo->id]);
			return (NULL);
		}
		sem_post(philo->table->activity_sem[philo->id]);
		usleep(100);
	}
	return (NULL);
}
