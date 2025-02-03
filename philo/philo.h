/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:25:24 by reemessam         #+#    #+#             */
/*   Updated: 2025/02/03 22:03:32 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// Include any necessary libraries here
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
// Define any constants or macros here

// Declare any function prototypes here
typedef struct s_rules	t_rules;
struct	s_rules
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_eat;
	int				finished_eating;
	long			start_time;
	int				available_slots;
	int				dead;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	arbiter_lock;
};

typedef struct s_philo	t_philo;
struct	s_philo
{
	int				id;
	int				times_eaten;
	long			last_meal;
	pthread_t		thread;
	t_rules			*rules;
};
//init.c
int     validate_rules(t_rules *rules);
int     parse_rules(t_rules *rules, int argc, char **argv);
int     init_mutexes(t_rules *rules);
int     init_rules(t_rules *rules, int argc, char **argv);
//main.c
void    cleanup(t_rules *rules, t_philo *philos);
//philo.c
void    *philo_routine(void *arg);
int     check_death_or_full(t_philo *philo, t_rules *rules);
void    perform_eating(t_philo *philo, t_rules *rules);
int     init_philos(t_rules *rules, t_philo **philos);
//utils.c
int     ft_atoi(const char *str);
long    get_timestamp(void);
void    print_action(t_philo *philo, char *action);
void    ft_usleep(t_rules *rules, long time_in_ms);
#endif /* PHILO_H */
