/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 14:24:50 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/07 14:34:42 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <semaphore.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/time.h>
# include <pthread.h>

# define FORKS_SEM "/forks_sem"
# define STOP_SEM "/stop_sem"
# define ACTIVITY_SEM "/activity_sem"
# define PRINT_SEM "/print_sem"
# define STOP_EAT_SEM "/stop_eat_sem"

typedef struct s_prep_end
{
	int					index;
	char				*name;
	char				*tmp;
}	t_prep_end;

typedef struct s_table
{
	int					nbr;
	int					dead;
	unsigned long long	start;
	int					stop;
	sem_t				*forks_sem;
	sem_t				**gen_sem;
	sem_t				*stop_sem;
	sem_t				**activity_sem;
	sem_t				*print_sem;
}	t_table;

typedef struct s_philo
{
	struct s_table		*table;
	int					id;
	char				*id_print;
	unsigned long long	die;
	int					eat;
	int					sleep;
	int					all_eat;
	int					eat_count;
	int					must_eat;
	int					dead;
	unsigned long long	last_meal;
	int					stop_eat;
}	t_philo;

// MAIN
int					main(int argc, char *argv[]);
int					check_input(int argc, char *argv[]);
int					prepare_table_philos(t_philo *philo,
						t_table *table, int argc, char *argv[]);
void				initialize_philosopher(t_philo *philo,
						t_table *table, int argc, char *argv[]);
int					initialize_sem_array(t_table *table);

// HANDLE_DINER
char				*prepare_name(char *sem_name, int index);
int					launch_diner(t_philo *philo);
void				routine(t_philo *philo, int id);
int					prepare_routine(t_philo *philo, pthread_t *general_monitor,
						pthread_t *death_monitor, int id);
void				*general_monitoring(void *arg);

// HANDLE_DINER_2
void				*death_monitoring(void *arg);
void				diner_starter(t_table *table);
int					check_continue_death(t_table *table);
int					end_diner(t_philo *philo, int end);
int					handle_sem_array(t_philo *philo,
						int end, t_prep_end prep_end);

// HANDLE_DINER_3
int					check_continue_eat(t_philo *philo);

// ACTIONS
int					print_action(t_philo *philo, char *message, int action);
char				*prepare_message(t_philo *philo, char *message);
char				*prepare_id_print(t_philo *philo, char *message);
void				communicate_death(t_philo *philo);

// ACTION2
int					forking(t_philo *philo);
int					eating(t_philo *philo);
int					sleeping_thinking(t_philo *philo);

// UTILS
unsigned long long	get_curr_time(void);
char				*ft_itoa(unsigned long long n);
int					ft_atoi(const char *nptr);

// UTILS2
size_t				ft_strlen(const char *s);
char				*ft_strjoin(char const *s1, char const *s2);
void				custom_usleep(t_table *table,
						unsigned long long time_sleep);
#endif
