/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erzhuo <erzhuo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:43:26 by erzhuo            #+#    #+#             */
/*   Updated: 2026/08/01 18:35:12 by erzhuo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	read_stop(t_table *table)
{
	int	i;

	pthread_mutex_lock(&table->stop_lock);
	i = table->stop;
	pthread_mutex_unlock(&table->stop_lock);
	return (i);
}

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
}

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_lock);
	print_status(philo, "is eating");
	ft_usleep(philo->table->time_to_eat);
	pthread_mutex_lock(&philo->meal_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	*one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->table->time_to_die);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->n_philo == 1)
		return (one_philo(philo));
	if (philo->id % 2 == 0)
		ft_usleep(philo->table->time_to_eat / 2);
	while (!read_stop(philo->table))
	{
		take_forks(philo);
		eat(philo);
		print_status(philo, "is sleeping");
		ft_usleep(philo->table->time_to_sleep);
		print_status(philo, "is thinking");
		if (philo->table->n_philo % 2 == 1)
			ft_usleep((philo->table->time_to_die
					- philo->table->time_to_eat
					- philo->table->time_to_sleep) / 2);
	}
	return (NULL);
}
