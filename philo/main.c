#include "philo.h"

int	main(int argc, char **argv)
{
	int		i;
	t_rules	rules;
	t_philo	*philos;

	if (init_rules(&rules, argc, argv) || init_philos(&rules, &philos))
		return (1);

	while (rules.dead == 0 && (rules.num_eat == 0
			|| rules.finished_eating < rules.num_philos))
	{
	}

	i = -1;
	while (++i < rules.num_philos)
	{
		pthread_detach(philos[i].thread);
		usleep(100);
	}
	// destroy
	i = -1;
	while (++i < rules.num_philos)
	{
		pthread_mutex_destroy(&rules.forks[i]);
		pthread_mutex_destroy(philos[i].left_fork);
		pthread_mutex_destroy(philos[i].right_fork);
	}
	usleep(100);

	pthread_mutex_destroy(&rules.print_mutex);
	pthread_mutex_destroy(&rules.dead_mutex);
	pthread_mutex_destroy(&rules.arbiter_lock);

	free(rules.forks);
	free(philos);
	return (0);
}
