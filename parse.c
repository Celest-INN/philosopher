/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erzhuo <erzhuo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:56:25 by erzhuo            #+#    #+#             */
/*   Updated: 2026/07/28 13:56:46 by erzhuo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static long	ft_atol(const char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		if (result > INT_MAX)
			return (-1);
		i++;
	}
	return (result);
}

static int	values_in_range(t_table *t, int argc)
{
	if (t->n_philo <= 0 || t->time_to_die <= 0
		|| t->time_to_eat <= 0 || t->time_to_sleep <= 0)
		return (0);
	if (argc == 6 && t->must_eat <= 0)
		return (0);
	return (1);
}

int	parse_args(t_table *table, int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
			return (error_msg("values must be positive"));
		i++;
	}
	table->n_philo = ft_atol(argv[1]);
	table->time_to_die = ft_atol(argv[2]);
	table->time_to_eat = ft_atol(argv[3]);
	table->time_to_sleep = ft_atol(argv[4]);
	if (argc == 6)
		table->must_eat = ft_atol(argv[5]);
	else
		table->must_eat = -1;
	if (!values_in_range(table, argc))
		return (error_msg("values must be positive and fit in int"));
	return (0);
}
