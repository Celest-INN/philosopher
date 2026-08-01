/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erzhuo <erzhuo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:43:30 by erzhuo            #+#    #+#             */
/*   Updated: 2026/07/28 14:21:50 by erzhuo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philo)
	{
		if (pthread_create(&table->philos[i].thread, NULL, routine,
				&table->philos[i]) != 0)
			return (error_msg("thread creation failed"));
		i++;
	}
	if (pthread_create(&table->monitor, NULL, monitor, table) != 0)
		return (error_msg("monitor creation failed"));
	return (0);
}

static void	join_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philo)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
	pthread_join(table->monitor, NULL);
}

int	start_simulation(t_table *table)
{
	if (create_threads(table))
		return (1);
	join_threads(table);
	return (0);
}
