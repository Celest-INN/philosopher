/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erzhuo <erzhuo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 14:01:31 by erzhuo            #+#    #+#             */
/*   Updated: 2026/07/28 14:02:49 by erzhuo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <string.h>
# include <limits.h>

typedef struct s_table	t_table;
typedef struct s_philo	t_philo;

struct	s_table
{
	int				n_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat;
	long			start_time;
	int				stop;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	stop_lock;
	pthread_t		monitor;
	t_philo			*philos;
};

struct s_philo
{
	int				id;
	long			last_meal;
	int				meals_eaten;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		thread;
	t_table			*table;
};

//utils.c
int		error_msg(const char *msg);
long	get_time(void);
void	ft_usleep(long ms);
void	print_status(t_philo *philo, char *status);

//cleanup.c
void	cleanup_table(t_table *table, int stage);

//parse.c
int		parse_args(t_table *table, int argc, char **argv);

//init.c
int		init_table(t_table *table);

//routine
int		read_stop(t_table *table);
void	*routine(void *arg);

//monitor.c
void	set_stop(t_table *table);
void	*monitor(void *arg);

//sim
int		start_simulation(t_table *table);

// 当前时间 - last_meal 超过 time_to_die 就是饿死了。
#endif