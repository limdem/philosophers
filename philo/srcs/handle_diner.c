/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_diner.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 02:03:55 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/05 02:18:13 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	routine_starter(philo);
	while (1)
	{
		if (forking(philo))
			return (NULL);
		if (eating(philo))
			return (NULL);
		if (check_continue_eat(philo))
			return (NULL);
		if (sleeping_thinking(philo))
			return (NULL);
		if (check_continue_death(philo))
			return (NULL);
		if (philo->table->nbr % 2)
			custom_usleep(philo->table, philo->sleep);
	}
	return (NULL);
}

int	check_all_eat(t_table *table, t_death_monitor monitor)
{
	pthread_mutex_lock(&table->all_eat_mutex);
	while (monitor.j < table->nbr)
	{
		monitor.all_eat = table->eat_count[monitor.j];
		monitor.eat_counter += monitor.all_eat;
		monitor.j++;
	}
	if (monitor.eat_counter == table->nbr)
	{
		pthread_mutex_lock(&table->print_mutex);
		table->stop = 1;
		pthread_mutex_unlock(&table->print_mutex);
		pthread_mutex_unlock(&table->all_eat_mutex);
		return (1);
	}
	pthread_mutex_unlock(&table->all_eat_mutex);
	return (0);
}

void	initialize_deat_monitoring(t_death_monitor *monitor, t_table *table)
{
	monitor->i = 0;
	monitor->j = 0;
	monitor->eat_counter = 0;
	monitor->all_eat = 0;
	diner_starter(table);
}

int	death_monitoring(t_philo *philo, t_death_monitor monitor)
{
	initialize_deat_monitoring(&monitor, philo->table);
	while (1)
	{
		if (check_all_eat(philo->table, monitor))
			return (0);
		pthread_mutex_lock(&philo->table->all_eat_mutex);
		monitor.continue_check = philo->table->eat_count[monitor.i];
		pthread_mutex_unlock(&philo->table->all_eat_mutex);
		if (!monitor.continue_check)
		{
			pthread_mutex_lock(philo->table->activity + monitor.i);
			if (get_curr_time() - philo->table->last_meals[monitor.i]
				> philo->table->die)
			{
				if (print_action(philo, "died\n", 0))
					return (1);
				pthread_mutex_unlock(philo->table->activity + monitor.i);
				return (0);
			}
			pthread_mutex_unlock(philo->table->activity + monitor.i);
		}
		next_check(philo->table->nbr, &monitor);
	}
	return (0);
}

int	launch_diner(t_philo *philo)
{
	t_death_monitor	monitor;
	int				i;

	i = 0;
	philo->table->start = get_curr_time() + philo->table->nbr * 25;
	while (i < philo->table->nbr)
	{
		if (pthread_create(&((philo + i)->t), NULL,
				&routine, (void *)(philo + i)) != 0)
			return (1);
		i++;
	}
	if (death_monitoring(philo, monitor))
		return (1);
	pthread_mutex_lock(&philo->table->death);
	philo->table->dead = 1;
	pthread_mutex_unlock(&philo->table->death);
	return (0);
}
