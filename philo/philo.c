/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:25:19 by reemessam         #+#    #+#             */
/*   Updated: 2025/02/06 18:28:47 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_rules	*rules;

	philo = (t_philo *)arg;
	rules = philo->rules;
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (check_death_or_full(philo, rules))
			return (NULL);
		perform_eating(philo, rules);
		print_action(philo, "is sleeping");
		ft_usleep(rules, rules->time_to_sleep);
		print_action(philo, "is thinking");
	}
}

static int	pickup_fork(t_philo *philo, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	print_action(philo, "has taken a fork");
	if (check_death(philo, philo->rules))
	{
		pthread_mutex_unlock(fork);
		return (FALSE);
	}
	return (TRUE);
}

void	perform_eating(t_philo *philo, t_rules *rules)
{
	if (philo->id % 2 == 0)
	{
		if (!pickup_fork(philo, &rules->forks[philo->id]))
			return ;
		if (!pickup_fork(philo, &rules->forks[(philo->id + 1)
				% rules->num_philos]))
		{
			pthread_mutex_unlock(&rules->forks[philo->id]);
			return ;
		}
	}
	else
	{
		if (!pickup_fork(philo, &rules->forks[(philo->id + 1)
				% rules->num_philos]))
			return ;
		if (!pickup_fork(philo, &rules->forks[philo->id]))
		{
			pthread_mutex_unlock(&rules->forks[(philo->id + 1)
				% rules->num_philos]);
			return ;
		}
	}
	philo->times_eaten++;
	philo->last_meal = get_timestamp();
	print_action(philo, "is eating");
	ft_usleep(rules, rules->time_to_eat);
	pthread_mutex_unlock(&rules->forks[(philo->id + 1) % rules->num_philos]);
	pthread_mutex_unlock(&rules->forks[philo->id]);
	pthread_mutex_lock(&rules->arbiter_lock);
	rules->available_slots++;
	pthread_mutex_unlock(&rules->arbiter_lock);
}

int	init_philos(t_rules *rules, t_philo **philos)

{
	int i;

	*philos = malloc(sizeof(t_philo) * rules->num_philos);
	if (!*philos)
		return (1);
	i = -1;
	while (++i < rules->num_philos)
	{
		(*philos)[i].id = i;
		(*philos)[i].times_eaten = 0;
		(*philos)[i].last_meal = rules->start_time;
		(*philos)[i].rules = rules;
		if (pthread_create(&(*philos)[i].thread, NULL, philo_routine,
				&(*philos)[i]))
			return (1);
	}
	return (0);
}
