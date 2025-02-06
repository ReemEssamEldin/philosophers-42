/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reldahli <reldahli@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:25:28 by reemessam         #+#    #+#             */
/*   Updated: 2025/02/06 18:27:24 by reldahli         ###   ########.fr       */
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

long	get_timestamp(void)
{
	struct timeval	tv;
	long			time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time_in_ms);
}

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
		usleep(10);
	}
}
