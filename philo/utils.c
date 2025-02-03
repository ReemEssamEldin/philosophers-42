/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:25:28 by reemessam         #+#    #+#             */
/*   Updated: 2025/02/03 22:05:09 by reldahli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign *= -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + *str - '0';
		str++;
	}
	return (res * sign);
}

/*
 * Get Current Timestamp
 * ---------------------
 * Returns the current time in milliseconds.
 *
 * Implementation:
 * - Uses gettimeofday() system call to get current time
 * - Converts seconds to milliseconds (sec * 1000)
 * - Adds microseconds converted to milliseconds (usec / 1000)
 *
 * Return Value:
 * - long: Current time in milliseconds
 *
 * Note: This function is used for tracking philosopher activities
 * and determining death conditions in the simulation
 */
long	get_timestamp(void)
{
	struct timeval	tv;
	long			time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time_in_ms);
}

/*
 * Print Philosopher Action
 * ------------------------
 * Thread-safe function to print philosopher state changes during simulation.
 *
 * Parameters:
 * @philo: Pointer to philosopher structure containing:
 *       - id: Philosopher's identifier
 *       - rules: Pointer to shared rules (includes print_mutex)
 * @action: String describing the philosopher's current action
 *
 * Implementation:
 * 1. Acquires print_mutex to ensure atomic printing
 * 2. Calculates relative timestamp from simulation start
 * 3. Checks death condition before printing
 * 4. Prints in format: "<timestamp> <philo_id> <action>"
 * 5. Releases print_mutex
 *
 * Note: Early return if simulation death condition is met
 */
void	print_action(t_philo *philo, char *action)
{
	long	timestamp;

	pthread_mutex_lock(&philo->rules->print_mutex);
	timestamp = get_timestamp() - philo->rules->start_time;
	if (philo->rules->dead)
	{
		pthread_mutex_unlock(&philo->rules->print_mutex);
		return ;
	}
	printf("%ld %d %s\n", timestamp, philo->id + 1, action);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

/*
 * Custom Sleep Function
 * -------------------
 * Provides millisecond-precision sleep functionality using usleep.
 *
 * Parameters:
 * @time_in_ms: Time to sleep in milliseconds
 *
 * Implementation:
 * - Converts milliseconds to microseconds (x1000)
 * - Uses system call usleep for suspension
 *
 * Note: Used throughout simulation for timing control
 * of philosopher actions (eating, sleeping)
 */
void	ft_usleep(t_rules *rules, long time_in_ms)
{
	long	start;
	long	elapsed;

	start = get_timestamp();
	elapsed = 0;
	while (1)
	{
		if (rules->dead)
		{
			return ;
		}
		elapsed = get_timestamp() - start;
		if (elapsed >= time_in_ms)
		{
			break ;
		}

		usleep(100);
	}
}
