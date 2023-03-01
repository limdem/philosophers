/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 13:58:26 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/07 14:01:41 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	initialize_sem_array(t_table *table)
{
	int		i;
	char	*name;

	i = 0;
	while (i < table->nbr)
	{
		name = prepare_name("/gen_sem", i);
		if (!name)
			return (1);
		table->gen_sem[i] = sem_open(name, O_CREAT | O_EXCL, 0700, 1);
		if (!table->gen_sem[i])
			return (1);
		sem_wait(table->gen_sem[i]);
		free(name);
		name = prepare_name("/activity_sem", i);
		if (!name)
			return (1);
		table->activity_sem[i] = sem_open(name, O_CREAT | O_EXCL, 0700, 1);
		if (!table->activity_sem[i])
			return (1);
		free(name);
		i++;
	}
	return (0);
}

void	initialize_philosopher(t_philo *philo, t_table *table,
				int argc, char *argv[])
{
	philo->table = table;
	philo->die = ft_atoi(argv[2]);
	philo->eat = ft_atoi(argv[3]);
	philo->sleep = ft_atoi(argv[4]);
	philo->all_eat = 0;
	philo->eat_count = 0;
	if (argc == 6)
		philo->must_eat = ft_atoi(argv[5]);
	else
		philo->must_eat = -1;
}

int	prepare_table_philos(t_philo *philo, t_table *table, int argc, char *argv[])
{
	initialize_philosopher(philo, table, argc, argv);
	table->nbr = ft_atoi(argv[1]);
	table->stop = 0;
	table->gen_sem = malloc(sizeof(sem_t) * table->nbr);
	table->activity_sem = malloc(sizeof(sem_t) * table->nbr);
	sem_unlink(FORKS_SEM);
	table->forks_sem = sem_open(FORKS_SEM, O_CREAT | O_EXCL, 0700, table->nbr);
	if (!table->forks_sem)
		return (1);
	sem_unlink(STOP_SEM);
	table->stop_sem = sem_open(STOP_SEM, O_CREAT | O_EXCL, 0700, 1);
	if (!table->stop_sem)
		return (1);
	sem_unlink(PRINT_SEM);
	table->print_sem = sem_open(PRINT_SEM, O_CREAT | O_EXCL, 0700, 1);
	if (!table->print_sem)
		return (1);
	if (initialize_sem_array(table))
		return (1);
	return (0);
}

int	check_input(int argc, char *argv[])
{
	int	i;
	int	j;

	if (!(argc == 5 || argc == 6))
	{
		printf("You entered a wrong input\n");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
			{
				printf("You entered a wrong input\n");
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_philo	philo;
	t_table	table;

	if (check_input(argc, argv))
		return (1);
	if (prepare_table_philos(&philo, &table, argc, argv))
		return (1);
	if (launch_diner(&philo))
		return (1);
	if (end_diner(&philo, 1))
		return (1);
	return (0);
}
// PARSING: VERIFIER QUE TOUS LES INPUTS SONT STRICTEMENT POSITIFS
// FAIRE COMMENCER LES ID A 1
