/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_diner.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 13:57:11 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/07 13:58:02 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*general_monitoring(void *arg)
{
	t_philo	*philo;

	philo = arg;
	sem_wait(philo->table->gen_sem[philo->id]);
	sem_wait(philo->table->stop_sem);
	philo->table->stop = 1;
	sem_post(philo->table->stop_sem);
	sem_post(philo->table->gen_sem[philo->id]);
	return (NULL);
}

int	prepare_routine(t_philo *philo, pthread_t *general_monitor,
	pthread_t *death_monitor, int id)
{
	philo->id = id;
	philo->last_meal = philo->table->start;
	if (pthread_create(general_monitor, NULL, &general_monitoring,
			(void *) philo) != 0)
		return (1);
	if (pthread_create(death_monitor, NULL, &death_monitoring, (void *) philo))
		return (1);
	diner_starter(philo->table);
	if (philo->id % 2)
		custom_usleep(philo->table, philo->eat);
	return (0);
}

void	routine(t_philo *philo, int id)
{
	pthread_t	death_monitor;
	pthread_t	general_monitor;

	if (prepare_routine(philo, &general_monitor, &death_monitor, id))
		return ;
	while (1)
	{
		if (forking(philo))
			return ;
		if (eating(philo))
			return ;
		if (check_continue_eat(philo))
			break ;
		if (sleeping_thinking(philo))
			return ;
		if (check_continue_death(philo->table))
			break ;
		if (philo->table->nbr % 2)
			custom_usleep(philo->table, philo->sleep);
	}
	pthread_join(general_monitor, NULL);
	pthread_join(death_monitor, NULL);
	if (end_diner(philo, 0))
		exit (1);
	exit(0);
}

int	launch_diner(t_philo *philo)
{
	int	pid;
	int	i;

	pid = 1;
	i = -1;
	philo->table->start = get_curr_time() + philo->table->nbr * 25;
	while (++i < philo->table->nbr)
	{
		if (pid)
		{
			pid = fork();
			if (!pid)
			{
				routine(philo, i);
				exit(0);
			}
		}
	}
	i = -1;
	while (++i < philo->table->nbr)
		wait(NULL);
	return (0);
}

char	*prepare_name(char *sem_name, int index)
{
	char	*name;
	char	*tmp;

	tmp = ft_itoa(index);
	if (!tmp)
		return (NULL);
	name = ft_strjoin(sem_name, tmp);
	free(tmp);
	if (!name)
		return (NULL);
	sem_unlink(name);
	return (name);
}
