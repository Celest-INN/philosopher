/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erzhuo <erzhuo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:43:51 by erzhuo            #+#    #+#             */
/*   Updated: 2026/07/28 13:52:22 by erzhuo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_forks(t_table *table)
{
	int	i;

	table->forks = malloc(sizeof(pthread_mutex_t) * table->n_philo);
	if (!table->forks)
		return (error_msg("malloc forks failed"));
	i = 0;
	while (i < table->n_philo)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&table->forks[i]);
			free(table->forks);
			return (error_msg("fork mutex init failed"));
		}
		i++;
	}
	return (0);
}

static void	set_philos(t_table *table, int i)
{
	table->philos[i].id = i + 1;
	table->philos[i].meals_eaten = 0;
	table->philos[i].last_meal = table->start_time;
	table->philos[i].table = table;
	table->philos[i].left_fork = &table->forks[i];
	table->philos[i].right_fork = &table->forks[(i + 1) % table->n_philo];
}

static int	init_philos(t_table *table)
{
	int	i;

	table->philos = malloc(sizeof(t_philo) * table->n_philo);
	if (!table->philos)
		return (error_msg("malloc philos failed"));
	i = 0;
	while (i < table->n_philo)
	{
		set_philos(table, i);
		if (pthread_mutex_init(&table->philos[i].meal_lock, NULL) != 0)
		{
			while (--i > 0)
				pthread_mutex_destroy(&table->philos[i].meal_lock);
			return (error_msg("meal mutex init failed"));
			free(table->philos);
		}
		i++;
	}
	return (0);
}
//&table->print_lock: table 里那把 print_lock 锁，在内存里的地址。

int	init_table(t_table *table)
{
	table->stop = 0;
	table->start_time = get_time();
	if (pthread_mutex_init(&table->print_lock, NULL) != 0)
		return (error_msg("print mutex init failed"));
	if (pthread_mutex_init(&table->stop_lock, NULL) != 0)
	{
		cleanup_table(table, 1);
		return (error_msg("stop mutex init failed"));
	}
	if (init_forks(table))
	{
		cleanup_table(table, 2);
		return (1);
	}
	if (init_philos(table))
	{
		cleanup_table(table, 3);
		return (1);
	}
	return (0);
}
