/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 11:41:58 by reldahli          #+#    #+#             */
/*   Updated: 2025/02/06 22:39:02 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo *philo, t_rules *rules)
{
	if (rules->dead)
	{
		return (TRUE);
	}
	if (philo->last_meal + rules->time_to_die < get_timestamp())
	{
		print_action(philo, "died");
		pthread_mutex_lock(&rules->dead_mutex);
		rules->dead = 1;
		pthread_mutex_unlock(&rules->dead_mutex);
		return (TRUE);
	}
	return (FALSE);
}

int	check_full(t_philo *philo, t_rules *rules)
{
	if (rules->num_eat > 0 && philo->times_eaten >= rules->num_eat)
	{
		pthread_mutex_lock(&rules->finished_mutex);
		if (!philo->done_eating)
		{
			rules->finished_eating++;
			philo->done_eating = 1;
		}
		pthread_mutex_unlock(&rules->finished_mutex);
		return (TRUE);
	}
	return (FALSE);
}

int	check_death_or_full(t_philo *philo, t_rules *rules)
{
	while (TRUE)
	{
		if (check_death(philo, rules) || check_full(philo, rules))
			return (TRUE);
		pthread_mutex_lock(&rules->arbiter_lock);
		if (rules->available_slots > 0)
		{
			rules->available_slots--;
			pthread_mutex_unlock(&rules->arbiter_lock);
			break ;
		}
		pthread_mutex_unlock(&rules->arbiter_lock);
		usleep(1);
	}
	return (FALSE);
}
