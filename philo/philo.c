/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:25:19 by reemessam         #+#    #+#             */
/*   Updated: 2025/02/03 15:52:26 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
* Philosopher Thread Routine
* --------------------------
* Main execution routine for each philosopher thread that implements
* the dining philosophers simulation lifecycle.
*
* Parameters:
* @arg: Void pointer to philosopher structure (cast to t_philo*)
*
* Lifecycle Loop:
* 1. Thinking Phase
*    - Announces thinking state
*    - Prepares to acquire forks
*
* 2. Death/Completion Check
*    - Verifies if philosopher should continue
*    - Exits if death occurred or eating quota met
*
* 3. Eating Phase
*    - Acquires forks (handled in perform_eating)
*    - Updates last meal timestamp
*    - Releases forks after eating duration
*
* 4. Sleeping Phase
*    - Announces sleep state
*    - Sleeps for specified duration
*
* Return Value:
* - NULL: Thread always returns NULL on completion
*
* Note: Loop continues indefinitely until death or completion condition met
*/
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
		ft_usleep(rules->time_to_sleep);
	}
}

/*
* Check Death or Full Condition
* -----------------------------
* Monitors philosopher's state for death or completion conditions
* and manages dining slot availability.
*
* Parameters:
* @philo: Pointer to philosopher structure containing:
*       - last_meal: Timestamp of last meal
*       - times_eaten: Count of meals eaten
* @rules: Pointer to shared rules containing:
*       - time_to_die: Maximum time between meals
*       - num_eat: Required number of meals
*       - available_slots: Current dining slots
*
* Checks:
* 1. Death Check:
*    - Compares time since last meal with time_to_die
*    - Sets death flag if exceeded
*
* 2. Completion Check:
*    - Verifies if required meals (num_eat) are completed
*    - Updates finished_eating counter
*
* 3. Slot Management:
*    - Monitors available dining slots
*    - Waits for slot availability
*
* Return Values:
* - 1: Death occurred or philosopher is full
* - 0: Can continue dining
*
* Note: Uses arbiter_lock for slot management synchronization
*/
int	check_death_or_full(t_philo *philo, t_rules *rules)
{
	while (1)
	{
		if (philo->last_meal + rules->time_to_die < get_timestamp())
		{
			print_action(philo, "died");
			pthread_mutex_lock(&rules->dead_mutex);
			rules->dead = 1;
			return (1);
		}
		if (rules->num_eat > 0 && philo->times_eaten >= rules->num_eat)
		{
			rules->finished_eating++;
			return (1);
		}
		pthread_mutex_lock(&rules->arbiter_lock);
		if (rules->available_slots > 0)
		{
			rules->available_slots--;
			pthread_mutex_unlock(&rules->arbiter_lock);
			break ;
		}
		pthread_mutex_unlock(&rules->arbiter_lock);
		usleep(100);
	}
	return (0);
}

/*
* Perform Eating Cycle
* -------------------
* Manages the entire eating process for a philosopher including
* fork acquisition, eating, and resource release.
*
* Parameters:
* @philo: Philosopher structure containing:
*       - id: Philosopher's identifier
*       - last_meal: Timestamp to update
*       - times_eaten: Meal counter
* @rules: Rules structure containing:
*       - forks: Array of fork mutexes
*       - time_to_eat: Duration of eating
*       - num_philos: For circular fork calculation
*
* Sequence:
* 1. Fork Acquisition:
*    - Locks left fork (philo->id)
*    - Locks right fork ((id + 1) % num_philos)
*
* 2. Eating Process:
*    - Updates last_meal timestamp
*    - Increments times_eaten counter
*    - Sleeps for time_to_eat duration
*
* 3. Resource Release:
*    - Unlocks both forks
*    - Updates available dining slots
*
* Note: Fork acquisition order prevents deadlocks
*/
void	perform_eating(t_philo *philo, t_rules *rules)
{
	pthread_mutex_lock(&rules->forks[philo->id]);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(&rules->forks[(philo->id + 1) % rules->num_philos]);
	print_action(philo, "has taken a fork");
	print_action(philo, "is eating");
	philo->last_meal = get_timestamp();
	philo->times_eaten++;
	ft_usleep(rules->time_to_eat);
	pthread_mutex_unlock(&rules->forks[philo->id]);
	pthread_mutex_unlock(&rules->forks[(philo->id + 1) % rules->num_philos]);
	pthread_mutex_lock(&rules->arbiter_lock);
	rules->available_slots++;
	pthread_mutex_unlock(&rules->arbiter_lock);
}

/*
* Initialize Philosophers
* -----------------------
* Allocates and initializes philosopher structures and creates their threads.
*
* Parameters:
* @rules: Pointer to simulation rules and shared resources
* @philos: Double pointer to array of philosopher structures
*
* Initialization per philosopher:
* 1. Unique ID (1-based indexing)
*    - Each philosopher gets sequential ID starting from 1
*    - Used for identification in status messages
*
* 2. Eating counter
*    - Tracks number of times philosopher has eaten
*    - Used to check if simulation should end (if num_eat specified)
*
* 3. Last meal timestamp
*    - Set initially to simulation start time
*    - Updated each time philosopher eats
*    - Used to determine if philosopher dies
*
* 4. Rules reference
*    - Points to shared simulation parameters
*    - Provides access to mutexes and timing values
*
* 5. Fork assignments (left and right)
*    - Left fork: Index matches philosopher's index
*    - Right fork: (index + 1) % num_philos
*    - Creates circular arrangement of forks
*
* 6. Thread creation with philo_routine
*    - Each philosopher runs in separate thread
*    - Passes philosopher struct as argument
*    - Handles errors if thread creation fails
*
* Note: Right fork index uses modulo to create circular table arrangement
*/
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
		(*philos)[i].left_fork = &rules->forks[i];
		(*philos)[i].right_fork = &rules->forks[(i + 1) % rules->num_philos];
		if (pthread_create(&(*philos)[i].thread,
			NULL, philo_routine, &(*philos)[i]))
			return (1);
	}
	return (0);
}
