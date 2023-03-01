/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 01:48:28 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/05 01:56:46 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

char	*prepare_message(t_table *table, char *id_print, char *message)
{
	char	*time;
	char	*tmp;
	char	*print;

	tmp = ft_strjoin(id_print, message);
	if (!tmp)
		return (NULL);
	pthread_mutex_lock(&table->print_mutex);
	time = ft_itoa((get_curr_time() - table->start));
	if (!time)
	{
		free(tmp);
		return (NULL);
	}
	print = ft_strjoin(time, tmp);
	free(time);
	free(tmp);
	if (!print)
	{
		free(tmp);
		free(time);
		return (NULL);
	}
	return (print);
}

int	print_action(t_philo *philo, char *message, int action)
{
	char	*print;

	print = prepare_message(philo->table, philo->id_print, message);
	if (!print)
		return (1);
	if (philo->table->stop)
	{
		pthread_mutex_unlock(&philo->table->print_mutex);
		free(print);
		return (0);
	}
	write(1, print, ft_strlen(print));
	if (action == 0)
		philo->table->stop = 1;
	pthread_mutex_unlock(&philo->table->print_mutex);
	free(print);
	if (action == 0)
	{
		pthread_mutex_lock(&philo->table->death);
		philo->table->dead = 1;
		pthread_mutex_unlock(&philo->table->death);
	}
	return (0);
}

int	sleeping_thinking(t_philo *philo)
{
	if (print_action(philo, "is sleeping\n", 3))
		return (1);
	custom_usleep(philo->table, philo->sleep);
	if (print_action(philo, "is thinking\n", 4))
		return (1);
	return (0);
}

int	eating(t_philo *philo)
{
	pthread_mutex_lock(philo->table->activity + philo->id);
	if (print_action(philo, "is eating\n", 2))
	{
		pthread_mutex_unlock(philo->table->m_forks + philo->forks.f_fork);
		pthread_mutex_unlock(philo->table->m_forks + philo->forks.s_fork);
		return (1);
	}
	philo->table->last_meals[philo->id] = get_curr_time();
	pthread_mutex_unlock(philo->table->activity + philo->id);
	custom_usleep(philo->table, philo->eat);
	pthread_mutex_unlock(philo->table->m_forks + philo->forks.s_fork);
	pthread_mutex_unlock(philo->table->m_forks + philo->forks.f_fork);
	return (0);
}

int	forking(t_philo *philo)
{
	pthread_mutex_lock(philo->table->m_forks + philo->forks.f_fork);
	if (print_action(philo, "has taken a fork\n", 1))
	{
		pthread_mutex_unlock(philo->table->m_forks + philo->forks.f_fork);
		return (1);
	}
	if (!(philo->table->nbr > 1))
	{
		pthread_mutex_unlock(philo->table->m_forks + philo->forks.f_fork);
		usleep(philo->die * 1000);
		return (1);
	}
	pthread_mutex_lock(philo->table->m_forks + philo->forks.s_fork);
	if (print_action(philo, "has taken a fork\n", 1))
	{
		pthread_mutex_unlock(philo->table->m_forks + philo->forks.f_fork);
		pthread_mutex_unlock(philo->table->m_forks + philo->forks.s_fork);
		return (1);
	}
	return (0);
}
