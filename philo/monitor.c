/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:49:20 by reldahli          #+#    #+#             */
/*   Updated: 2025/02/06 21:58:24 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_monitor	*monitor;
	int			i;
	long		current_time;

	monitor = (t_monitor *)arg;
	while (1)
	{
		i = -1;
		while (++i < monitor->rules->num_philos)
		{
			current_time = get_timestamp();
			if (monitor->rules->dead)
				return (NULL);
			pthread_mutex_lock(&monitor->rules->dead_mutex);
			if (current_time
				- monitor->philos[i].last_meal >= monitor->rules->time_to_die)
			{
				print_action(&monitor->philos[i], "died");
				monitor->rules->dead = 1;
				pthread_mutex_unlock(&monitor->rules->dead_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&monitor->rules->dead_mutex);
			if (monitor->rules->num_eat > 0
				&& monitor->rules->finished_eating == monitor->rules->num_philos)
				return (NULL);
		}
		usleep(1000); // Sleep for 1ms to prevent excessive CPU usage
	}
	return (NULL);
}

int	create_monitor(t_rules *rules, t_philo *philos, pthread_t *monitor_thread)
{
	t_monitor	*monitor_data;

	monitor_data = malloc(sizeof(t_monitor));
	if (!monitor_data)
		return (FALSE);
	monitor_data->rules = rules;
	monitor_data->philos = philos;
	if (pthread_create(monitor_thread, NULL, monitor_routine, monitor_data))
	{
		free(monitor_data);
		return (FALSE);
	}
	return (TRUE);
}
