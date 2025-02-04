/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:25:08 by reemessam         #+#    #+#             */
/*   Updated: 2025/02/04 14:32:49 by reldahli         ###   ########.fr       */
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
	free(rules->forks);
	free(philos);
}

int	main(int argc, char **argv)
{
	t_rules	rules;
	t_philo	*philos;

	if (init_rules(&rules, argc, argv) || init_philos(&rules, &philos))
		return (1);
	while (1)
	{
		if (rules.dead == 1)
		{
			break ;
		}
		if (rules.num_eat > 0 && rules.finished_eating == rules.num_philos)
		{
			break ;
		}
		ft_usleep(&rules, 50);
	}
	cleanup(&rules, philos);
	return (0);
}
