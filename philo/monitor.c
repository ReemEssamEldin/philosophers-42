/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:49:20 by reldahli          #+#    #+#             */
/*   Updated: 2025/02/06 22:04:59 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_rules	*rules;
	int		i;
	long	current_time;

	philos = (t_philo *)arg;
	rules = philos[0].rules;
	while (1)
	{
		i = -1;
		while (++i < rules->num_philos)
		{
			current_time = get_timestamp();
			if (rules->dead)
				return (NULL);
			pthread_mutex_lock(&rules->dead_mutex);
			if (current_time - philos[i].last_meal >= rules->time_to_die)
			{
				print_action(&philos[i], "died");
				rules->dead = 1;
				pthread_mutex_unlock(&rules->dead_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&rules->dead_mutex);
			if (rules->num_eat > 0
				&& rules->finished_eating == rules->num_philos)
				return (NULL);
		}
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
