/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 09:47:41 by reldahli          #+#    #+#             */
/*   Updated: 2025/02/03 11:21:14 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
* Validate Simulation Rules
* --------------------------
* Performs validation checks on the parsed simulation parameters
* to ensure they meet the required constraints.
*
* Parameters:
* @rules: Pointer to rules structure containing:
*       - num_philos: Number of philosophers
*       - time_to_die: Time until philosopher dies without eating
*       - time_to_eat: Time taken to eat
*       - time_to_sleep: Time taken to sleep
*       - num_eat: Optional number of times each philosopher must eat
*
* Note: All time values are expected to be in milliseconds
*/
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

/*
* Parse Command Line Arguments
* ----------------------------
* Processes and validates command line arguments to initialize
* the simulation rules structure.
*
* Parameters:
* @rules: Pointer to rules structure to store parsed values
* @argc: Number of command line arguments (expected: 5 or 6)
* @argv: Array of command line argument strings:
*       [1] number_of_philosophers
*       [2] time_to_die (in milliseconds)
*       [3] time_to_eat (in milliseconds)
*       [4] time_to_sleep (in milliseconds)
*       [5] [optional] number_of_times_each_philosopher_must_eat
*
* Return Values:
* - 0: Arguments successfully parsed and validated
* - 1: Error in arguments (wrong count or invalid values)
*/
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

/*
* Initialize Mutexes
* -------------------
* Sets up all synchronization primitives (mutexes) needed for the simulation.
*
* Parameters:
* @rules: Pointer to rules structure containing simulation parameters
*
* Initialization sequence:
* 1. Reset control variables (finished_eating, available_slots)
* 2. Set simulation start time
* 3. Allocate and initialize fork mutexes
* 4. Initialize control mutexes:
*    - print_mutex: Controls console output
*    - dead_mutex: Guards death checking
*    - arbiter_lock: Controls philosopher access to forks
*
* Note: Each fork is represented by a mutex to prevent
* simultaneous access by multiple philosophers
*/
int	init_mutexes(t_rules *rules)
{
	int	i;

	rules->finished_eating = 0;
	rules->available_slots = rules->num_philos - 1;
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

/*
* Initialize Rules for Philosophers Simulation
* ---------------------------------------------
* This function sets up the core rules and synchronization primitives
* for the dining philosophers simulation.
*
* Parameters:
* @rules: Pointer to rules structure to be initialized
* @argc: Number of command line arguments
* @argv: Array of command line argument strings containing:
*       - number_of_philosophers
*       - time_to_die
*       - time_to_eat
*       - time_to_sleep
*       - [optional] number_of_times_each_philosopher_must_eat
*
* Function Flow:
* 1. Parse and validate command line arguments
* 2. Initialize all required mutexes
*/
int	init_rules(t_rules *rules, int argc, char **argv)
{
	if (parse_rules(rules, argc, argv))
		return (1);
	if (init_mutexes(rules))
		return (1);
	return (0);
}
