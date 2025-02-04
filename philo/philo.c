/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:25:19 by reemessam         #+#    #+#             */
/*   Updated: 2025/02/04 14:32:43 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_rules	*rules;

	philo = (t_philo *)arg;
	rules = philo->rules;
	while (1)
	{
		print_action(philo, "is thinking");
		if (check_death_or_full(philo, rules))
			return (NULL);
		perform_eating(philo, rules);
		print_action(philo, "is sleeping");
		ft_usleep(rules, rules->time_to_sleep);
	}
}

void	perform_eating(t_philo *philo, t_rules *rules)
{
	pthread_mutex_lock(&rules->forks[philo->id]);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(&rules->forks[(philo->id + 1) % rules->num_philos]);
	print_action(philo, "has taken a fork");
	print_action(philo, "is eating");
	philo->last_meal = get_timestamp();
	philo->times_eaten++;
	ft_usleep(rules, rules->time_to_eat);
	pthread_mutex_unlock(&rules->forks[philo->id]);
	pthread_mutex_unlock(&rules->forks[(philo->id + 1) % rules->num_philos]);
	pthread_mutex_lock(&rules->arbiter_lock);
	rules->available_slots++;
	pthread_mutex_unlock(&rules->arbiter_lock);
}

int	init_philos(t_rules *rules, t_philo **philos)

{
	int	i;

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
