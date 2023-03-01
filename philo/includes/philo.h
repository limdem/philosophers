/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 02:33:16 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/08 10:42:35 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdio.h>
# include <string.h>

typedef struct s_prep_philo
{
	int		i;
	char	*tmp_id;
	char	*tmp_str;
}	t_prep_philo;

typedef struct s_death_monitor
{
	int	i;
	int	j;
	int	all_eat;
	int	eat_counter;
	int	continue_check;
}	t_death_monitor;

typedef struct s_printer
{
	unsigned long long	time;
	int					id;
	int					message;
	struct s_printer	*next;
}	t_printer;

typedef struct s_table
{
	pthread_t			death_monitor;
	int					nbr;
	unsigned long long	start;
	int					dead;
	int					must_eat;
	int					all_eat;
	unsigned long long	die;
	int					stop;
	pthread_mutex_t		*m_forks;
	pthread_mutex_t		death;
	unsigned long long	*last_meals;
	int					*eat_count;
	pthread_mutex_t		*activity;
	pthread_mutex_t		all_eat_mutex;
	pthread_mutex_t		print_mutex;
}	t_table;

typedef struct s_forks
{
	int	l_fork;
	int	r_fork;
	int	f_fork;
	int	s_fork;
}	t_forks;

typedef struct s_philo
{
	struct s_table	*table;
	pthread_t		t;
	int				id;
	char			*id_print;
	int				die;
	int				eat;
	int				sleep;
	int				all_eat;
	struct s_forks	forks;
	int				eat_count;
}	t_philo;

// ACTIONS
char				*prepare_message(t_table *table,
						char *id_print, char *message);
int					print_action(t_philo *philo,
						char *message, int action);
int					sleeping_thinking(t_philo *philo);
int					eating(t_philo *philo);
int					forking(t_philo *philo);

// HANDLE_DINER
void				*routine(void *arg);
int					check_all_eat(t_table *table, t_death_monitor monitor);
void				initialize_death_monitoring(t_death_monitor *monitor,
						t_table *table);
int					death_monitoring(t_philo *philo,
						t_death_monitor monitor);
int					launch_diner(t_philo *philo);

// HANDLE_DINER2
void				end_diner(t_philo *philo, t_table *table);
void				prepare_forks(int id, t_forks *forks, int nbr);
void				routine_starter(t_philo *philo);
int					check_continue_death(t_philo *philo);
void				diner_starter(t_table *table);

// HANDLE_DINER3
int					check_continue_eat(t_philo *philo);
void				next_check(int nbr, t_death_monitor *monitor);

// MAIN
int					prepare_philosophers(t_philo **philo,
						t_table *table, char *argv[]);
int					initialize_table(t_table *table);
int					prepare_table(t_philo **philo,
						t_table *table, int argc, char *argv[]);
int					check_input(int argc, char *argv[]);

// UTILS
char				*ft_itoa(unsigned long long n);
char				*ft_strjoin(char const *s1, char const *s2);
int					ft_atoi(const char *nptr);

// UTILS2
unsigned long long	get_curr_time(void);
void				custom_usleep(t_table *table,
						unsigned long long time_sleep);
size_t				ft_strlen(const char *s);
char				*ft_strdup(const char *s);
size_t				ft_strlen(const char *s);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_itoa(unsigned long long n);
int					ft_atoi(const char *nptr);
#endif
