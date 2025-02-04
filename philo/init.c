/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 09:47:41 by reldahli          #+#    #+#             */
/*   Updated: 2025/02/04 18:14:58 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_rules(t_rules *rules)
{
	if (rules->num_philos < 0 || rules->time_to_die < 0
		|| rules->time_to_eat < 0 || rules->time_to_sleep < 0
		|| rules->num_eat < 0)
	{
		printf("Error: Negative values are not allowed\n");
		return (1);
	}
	return (0);
}

int	parse_rules(t_rules *rules, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	rules->num_philos = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		rules->num_eat = ft_atoi(argv[5]);
	else
		rules->num_eat = 0;
	if (validate_rules(rules))
		return (1);
	return (0);
}

int	init_mutexes(t_rules *rules)
{
	int	i;

	rules->finished_eating = 0;
	rules->available_slots = rules->num_philos / 2;
	rules->dead = 0;
	rules->start_time = get_timestamp();
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philos);
	if (!rules->forks)
		return (1);
	i = 0;
	while (i < rules->num_philos)
	{
		pthread_mutex_init(&rules->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&rules->print_mutex, NULL);
	pthread_mutex_init(&rules->dead_mutex, NULL);
	pthread_mutex_init(&rules->arbiter_lock, NULL);
	return (0);
}

int	init_rules(t_rules *rules, int argc, char **argv)
{
	if (parse_rules(rules, argc, argv))
		return (1);
	if (init_mutexes(rules))
		return (1);
	return (0);
}
