/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erzhuo <erzhuo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:43:42 by erzhuo            #+#    #+#             */
/*   Updated: 2026/07/28 14:00:50 by erzhuo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_msg(const char *msg)
{
	printf("Error: %s\n", msg);
	return (1);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(long ms)
{
	long	start;

	start = get_time();
	while (get_time() - start < ms)
		usleep(500);
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->table->stop_lock);
	if (philo->table->stop)
	{
		pthread_mutex_unlock(&philo->table->stop_lock);
		pthread_mutex_unlock(&philo->table->print_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->table->stop_lock);
	timestamp = get_time() - philo->table->start_time;
	printf("%ld %d %s\n", timestamp, philo->id, status);
	pthread_mutex_unlock(&philo->table->print_lock);
}
