#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_rules	*rules;

	philo = (t_philo *)arg;
	rules = philo->rules;
	while (1)
	{
		// Thinking
		print_action(philo, "is thinking");

		// Busy-wait until a slot is available
		while (1)
		{
			if (philo->last_meal + rules->time_to_die < get_timestamp())
			{
				print_action(philo, "died");
				pthread_mutex_lock(&rules->dead_mutex);
				rules->dead = 1;
				return (NULL);
			}

			if (rules->num_eat > 0 && philo->times_eaten >= rules->num_eat)
			{
				rules->finished_eating++;
				return (NULL);
			}

			pthread_mutex_lock(&rules->arbiter_lock);
			if (rules->available_slots > 0)
			{
				rules->available_slots--; // Occupy a slot
				pthread_mutex_unlock(&rules->arbiter_lock);
				break ;
			}
			pthread_mutex_unlock(&rules->arbiter_lock);
			// Small delay to reduce CPU usage in the busy-wait loop
			usleep(100);
		}

		// Pick up left and right forks
		pthread_mutex_lock(&rules->forks[philo->id]); // Left fork
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(&rules->forks[(philo->id + 1) % rules->num_philos]); // Right fork
		print_action(philo, "has taken a fork");

		// Eating
		print_action(philo, "is eating");
		philo->last_meal = get_timestamp(); // Updated line
		philo->times_eaten++;
		ft_usleep(rules->time_to_eat);

		// Put down forks
		pthread_mutex_unlock(&rules->forks[philo->id]);                           // Release left fork
		pthread_mutex_unlock(&rules->forks[(philo->id + 1) % rules->num_philos]); // Release right fork

		// Release arbiter permission
		pthread_mutex_lock(&rules->arbiter_lock);
		rules->available_slots++; // Free up a slot
		pthread_mutex_unlock(&rules->arbiter_lock);

		// Sleeping
		print_action(philo, "is sleeping");
		ft_usleep(rules->time_to_sleep);
	}
}

int	init_philos(t_rules *rules, t_philo **philos)

{
	int	i;

	*philos = malloc(sizeof(t_philo) * rules->num_philos);
	if (!*philos)
		return (1);
	i = -1;
	while (++i < rules->num_philos)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].times_eaten = 0;
		(*philos)[i].last_meal = rules->start_time; // Updated line
		(*philos)[i].rules = rules;
		(*philos)[i].left_fork = &rules->forks[i];
		(*philos)[i].right_fork = &rules->forks[(i + 1) % rules->num_philos];
		if (pthread_create(&(*philos)[i].thread,
			NULL, philo_routine, &(*philos)[i]))
			return (1);
	}
	return (0);
}
