/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:25:08 by reemessam         #+#    #+#             */
/*   Updated: 2025/02/03 22:02:21 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Cleanup Function
 * -----------------
 * Handles proper termination of the simulation by:
 * 1. Detaching philosopher threads
 * 2. Destroying all mutexes (forks and control mutexes)
 * 3. Freeing allocated memory
 *
 * Parameters:
 * @rules: Pointer to rules structure containing simulation parameters
 *         and synchronization primitives
 * @philos: Array of philosopher structures to be cleaned up
 *
 * Implementation details:
 * - First loop: Detaches all philosopher threads
 * - Second loop: Destroys fork mutexes and their references
 * - Final phase: Destroys control mutexes (print, dead, arbiter)
 * - Memory cleanup: Frees fork array and philosophers array
 *
 * Note: Small sleep delays (100us) are used to ensure proper
 *       cleanup sequencing
 */
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

/*
Flow:
* 1. Initialize rules and philosophers structures
* 2. Start simulation loop that continues until:
*    - A philosopher dies (rules.dead = 1) OR
*    - All philosophers have eaten required times (if num_eat specified)
* 3. Clean up allocated resources
*/
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
