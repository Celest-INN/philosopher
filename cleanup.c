/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erzhuo <erzhuo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:43:08 by erzhuo            #+#    #+#             */
/*   Updated: 2026/07/28 13:49:11 by erzhuo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	destroy_forks(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free (table->forks);
}

static void	destroy_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_destroy(&table->philos[i].meal_lock);
		i++;
	}
	free (table->philos);
}

void	cleanup_table(t_table *table, int stage)
{
	if (stage >= 4)
		destroy_philos(table);
	if (stage >= 3)
		destroy_forks(table);
	if (stage >= 2)
		pthread_mutex_destroy(&table->stop_lock);
	if (stage >= 1)
		pthread_mutex_destroy(&table->print_lock);
}
