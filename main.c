/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erzhuo <erzhuo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:43:58 by erzhuo            #+#    #+#             */
/*   Updated: 2026/08/01 15:01:09 by erzhuo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// int main(int ac, char **av)
// {
//     t_table table;
//     if (ac != 5 && ac != 6)
//         return (error_msg("usage: n_philo die eat sleep [must_eat]"));
//     if (parse_args(&table, ac, av))
//         return (1);
//     printf (" n=%d\n die=%ld\n eat=%ld\n sleep=%ld\n must_eat=%d\n",
//         table.n_philo, table.time_to_die, table.time_to_eat, 
//         table.time_to_sleep, table.must_eat);
//     return (0);
// }

// int main(int ac, char **av)
// {
//     t_table table;
//     int i;

//     i = 0;
//     if (ac != 5 && ac != 6)
//         return (error_msg("usage: n_philo die eat sleep [must_eat]"));
//     if (parse_args(&table, ac, av))
//         return (1);
//     if (init_table(&table))
//         return (1);
//     while (i < table.n_philo)
// 	{
// 		printf("philo %d: left=%p right=%p last_meal=%ld\n",
// 			table.philos[i].id, (void *)table.philos[i].left_fork,
// 			(void *)table.philos[i].right_fork, table.philos[i].last_meal);
// 		i++;
// 	}
//     cleanup_table(&table, 4);
//     return (0);
// }

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 5 && ac != 6)
		return (error_msg("usage: n_philo die eat sleep [must_eat]"));
	if (parse_args(&table, ac, av))
		return (1);
	if (init_table(&table))
		return (1);
	start_simulation(&table);
	cleanup_table(&table, 4);
	return (0);
}
