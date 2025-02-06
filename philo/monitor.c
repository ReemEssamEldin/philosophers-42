/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:49:20 by reldahli          #+#    #+#             */
/*   Updated: 2025/02/06 23:06:51 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <unistd.h>

static int	check_philosophers(t_philo *philos, t_rules *rules)
{
	int		i;
	long	current_time;

	i = -1;
	while (++i < rules->num_philos)
	{
		current_time = get_timestamp();
		if (rules->dead)
			return (1);
		if (rules->num_eat != 0 && philos[i].times_eaten >= rules->num_eat)
			continue ;
		pthread_mutex_lock(&rules->dead_mutex);
		if (current_time - philos[i].last_meal >= rules->time_to_die)
		{
			print_action(&philos[i], "died");
			rules->dead = 1;
			pthread_mutex_unlock(&rules->dead_mutex);
			return (1);
		}
		pthread_mutex_unlock(&rules->dead_mutex);
		if (rules->num_eat > 0 && rules->finished_eating == rules->num_philos)
			return (1);
	}
	return (0);
}

static int	check_all_finished(t_rules *rules)
{
	int	all_done;

	if (rules->num_eat <= 0)
		return (0);
	pthread_mutex_lock(&rules->finished_mutex);
	all_done = (rules->finished_eating == rules->num_philos);
	pthread_mutex_unlock(&rules->finished_mutex);
	if (all_done)
	{
		pthread_mutex_lock(&rules->dead_mutex);
		rules->dead = 1;
		pthread_mutex_unlock(&rules->dead_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_rules	*rules;

	philos = (t_philo *)arg;
	rules = philos[0].rules;
	while (1)
	{
		if (check_philosophers(philos, rules))
			return (NULL);
		if (check_all_finished(rules))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

int	create_monitor(t_philo *philos, pthread_t *monitor_thread)
{
	if (pthread_create(monitor_thread, NULL, monitor_routine, philos))
	{
		return (FALSE);
	}
	return (TRUE);
}
