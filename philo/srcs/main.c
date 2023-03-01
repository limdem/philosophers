/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 02:19:42 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/05 04:35:45 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	prepare_philosophers(t_philo **philo, t_table *table, char *argv[])
{
	t_prep_philo	prep_philo;

	prep_philo.i = -1;
	while (++prep_philo.i < table->nbr)
	{
		(*philo + prep_philo.i)->table = table;
		(*philo + prep_philo.i)->id = prep_philo.i;
		prep_philo.tmp_id = ft_itoa((unsigned long long) prep_philo.i + 1);
		if (!prep_philo.tmp_id)
			return (1);
		prep_philo.tmp_str = ft_strjoin(" ", prep_philo.tmp_id);
		free(prep_philo.tmp_id);
		if (!prep_philo.tmp_str)
			return (1);
		(*philo + prep_philo.i)->id_print = ft_strjoin(prep_philo.tmp_str, " ");
		free(prep_philo.tmp_str);
		(*philo + prep_philo.i)->die = table->die;
		(*philo + prep_philo.i)->eat = ft_atoi(argv[3]);
		(*philo + prep_philo.i)->sleep = ft_atoi(argv[4]);
		(*philo + prep_philo.i)->all_eat = 0;
		(*philo + prep_philo.i)->eat_count = 0;
	}
	return (0);
}

int	initialize_table(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nbr)
	{
		if (pthread_mutex_init(table->m_forks + i, NULL) != 0)
			return (1);
		if (pthread_mutex_init(table->activity + i, NULL) != 0)
			return (1);
		table->eat_count[i] = 0;
		i++;
	}
	pthread_mutex_init(&(table->death), NULL);
	pthread_mutex_init(&(table->print_mutex), NULL);
	pthread_mutex_init(&table->all_eat_mutex, NULL);
	return (0);
}

int	prepare_table(t_philo **philo,
		t_table *table, int argc, char *argv[])
{
	table->nbr = ft_atoi(argv[1]);
	table->m_forks = malloc(sizeof(pthread_mutex_t) * table->nbr);
	table->activity = malloc(sizeof(pthread_mutex_t) * table->nbr);
	table->last_meals = malloc(sizeof(unsigned long long) * table->nbr);
	table->eat_count = malloc(sizeof(int) * table->nbr);
	table->die = ft_atoi(argv[2]);
	table->all_eat = 0;
	table->dead = 0;
	table->stop = 0;
	if (argc == 6)
		table->must_eat = ft_atoi(argv[5]);
	else
		table->must_eat = -1;
	if (!table->m_forks || !table->activity
		|| !table->last_meals || !table->eat_count)
		return (1);
	initialize_table(table);
	*philo = malloc(sizeof(t_philo) * (table->nbr));
	if (!(*philo))
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
	t_philo	*philo;
	t_table	table;

	if (check_input(argc, argv))
		return (1);
	if (prepare_table(&philo, &table, argc, argv))
		return (1);
	if (prepare_philosophers(&philo, &table, argv))
		return (1);
	if (launch_diner(philo))
		return (1);
	end_diner(philo, &table);
	return (0);
}
