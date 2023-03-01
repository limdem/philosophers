/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 13:55:45 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/07 13:56:05 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	communicate_death(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->table->nbr)
	{
		sem_post(philo->table->gen_sem[i]);
		i++;
	}
}

char	*prepare_id_print(t_philo *philo, char *message)
{
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;
	char	*id_print;

	tmp1 = ft_itoa(philo->id + 1);
	if (!tmp1)
		return (NULL);
	tmp2 = ft_strjoin(" ", tmp1);
	free(tmp1);
	if (!tmp2)
		return (NULL);
	tmp3 = ft_strjoin(tmp2, " ");
	free(tmp2);
	if (!tmp3)
		return (NULL);
	id_print = ft_strjoin(tmp3, message);
	free(tmp3);
	if (!id_print)
		return (NULL);
	sem_wait(philo->table->print_sem);
	return (id_print);
}

char	*prepare_message(t_philo *philo, char *message)
{
	char	*time;
	char	*print;

	philo->id_print = prepare_id_print(philo, message);
	if (!philo->id_print)
		return (NULL);
	time = ft_itoa((get_curr_time() - philo->table->start));
	if (!time)
		return (NULL);
	print = ft_strjoin(time, philo->id_print);
	free(time);
	free(philo->id_print);
	if (!print)
		return (NULL);
	return (print);
}

int	print_action(t_philo *philo, char *message, int action)
{
	char	*print;

	print = prepare_message(philo, message);
	if (!print)
		return (1);
	sem_wait(philo->table->stop_sem);
	if (philo->table->stop)
	{
		sem_post(philo->table->stop_sem);
		sem_post(philo->table->print_sem);
		free(print);
		return (0);
	}
	sem_post(philo->table->stop_sem);
	write(1, print, ft_strlen(print));
	if (action == 0)
	{
		sem_wait(philo->table->stop_sem);
		philo->table->stop = 1;
		sem_post(philo->table->stop_sem);
		communicate_death(philo);
	}
	sem_post(philo->table->print_sem);
	free(print);
	return (0);
}
