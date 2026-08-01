/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erzhuo <erzhuo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:43:20 by erzhuo            #+#    #+#             */
/*   Updated: 2026/08/01 15:32:21 by erzhuo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_stop(t_table *table)
{
	pthread_mutex_lock(&table->stop_lock);
	table->stop = 1;
	pthread_mutex_unlock(&table->stop_lock);
}

static void	announce_death(t_philo *philo)
{
	long	timestamp;

	pthread_mutex_lock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->table->stop_lock);
	philo->table->stop = 1;
	timestamp = get_time() - philo->table->start_time;
	printf("%ld %d died\n", timestamp, philo->id);
	pthread_mutex_unlock(&philo->table->stop_lock);
	pthread_mutex_unlock(&philo->table->print_lock);
}

static int	check_death(t_table *table)
{
	int		i;
	long	last;

	i = 0;
	last = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_lock(&table->philos[i].meal_lock);
		last = table->philos[i].last_meal;
		pthread_mutex_unlock(&table->philos[i].meal_lock);
		if (get_time() - last > table->time_to_die)
		{
			announce_death(&table->philos[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	all_ate_enough(t_table *table)
{
	int	i;
	int	full;

	if (table->must_eat == -1)
		return (0);
	i = 0;
	full = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_lock(&table->philos[i].meal_lock);
		if (table->philos[i].meals_eaten >= table->must_eat)
			full++;
		pthread_mutex_unlock(&table->philos[i].meal_lock);
		i++;
	}
	return (full == table->n_philo);
}

void	*monitor(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (1)
	{
		if (check_death(table))
			return (NULL);
		if (all_ate_enough(table))
		{
			set_stop(table);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
