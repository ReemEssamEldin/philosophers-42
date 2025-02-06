/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:25:08 by reldahli         #+#    #+#             */
/*   Updated: 2025/02/06 18:48:21 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup(t_rules *rules, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < rules->num_philos)
	{
		pthread_detach(philos[i].thread);
		usleep(100);
	}
	i = -1;
	while (++i < rules->num_philos)
	{
		pthread_mutex_destroy(&rules->forks[i]);
	}
	usleep(100);
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->dead_mutex);
	pthread_mutex_destroy(&rules->arbiter_lock);
	pthread_mutex_destroy(&rules->finished_mutex);
	free(rules->forks);
	free(philos);
}

int	main(int argc, char **argv)
{
	t_rules		rules;
	t_philo		*philos;
	pthread_t	monitor_thread;

	if (!init_rules(&rules, argc, argv))
		return (EXIT_FAILURE);
	if (!init_philos(&rules, &philos))
	{
		return (EXIT_FAILURE);
	}
	if (!create_monitor(philos, &monitor_thread))
	{
		cleanup(&rules, philos);
		return (EXIT_FAILURE);
	}
	pthread_join(monitor_thread, NULL);
	cleanup(&rules, philos);
	return (EXIT_SUCCESS);
}
