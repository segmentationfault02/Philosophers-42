/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marguima <marguima@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 17:58:59 by marguima          #+#    #+#             */
/*   Updated: 2026/01/25 15:06:55 by marguima         ###   ########.fr       */
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
# include <stdbool.h>
# include <errno.h>
# include <stdint.h>

typedef pthread_mutex_t	t_mutex;
typedef struct s_table	t_table;
# define MS_TO_US 1e3

/* ETATS PHILOS */
typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}			t_philo_status;

/* CODES POUR GET TIME*/
typedef enum e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}			t_time_code;

/* MUTEX ENUM */
typedef enum e_fcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_fcode;

/* FORKS */
typedef struct s_fork
{
	t_mutex	fork;
	int		fork_id;
}	t_fork;

/* PHILOS */
typedef struct s_philo
{
	int			id;
	long		meals_counter;
	bool		full;
	long		last_meal_time;
	t_fork		*first_fork; // left
	t_fork		*second_fork; // right
	pthread_t	thread_id;
	t_mutex		philo_mutex;
	t_table		*table;
}	t_philo;

/* TABLE */
struct s_table
{
	long		philo_nbr;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		nbr_limit_meals;
	long		start_simulation;
	bool		end_simulation; // Quand un philo meurt
	bool		all_ready;
	long		threads_running_nb;
	pthread_t	monitor;
	t_mutex		table_mutex;
	t_mutex		write_lock;
	t_fork		*forks;
	t_philo		*philos;
};

/* MAIN FUNCTIONS */
void	parse_input(t_table *table, char **av);
void	data_init(t_table *table);
void	dinner_start(t_table *table);
void	clean(t_table *table);

/* SAFE FUNCTIONS */
void	*safe_malloc(size_t bytes);
void	handle_mutex_error(int status, t_fcode fcode);
void	*safe_handle_mutex(t_mutex *mutex, t_fcode fcode);
void	handle_thread_error(int status, t_fcode fcode);
void	safe_thread_handle(pthread_t *thread, void *(*foo)(void *),
			void *arg, t_fcode fcode);

/* GETTERS ET SETTERS */
void	set_bool(t_mutex *mutex, bool *dest, bool value);
bool	get_bool(t_mutex *mutex, bool *value);
void	set_long(t_mutex *mutex, long *dest, long value);
long	get_long(t_mutex *mutex, long *value);
bool	simulation_finished(t_table *table);

/* UTILS */
void	error(char *str);
long	get_time(t_time_code timecode);
void	write_status(t_philo_status status, t_philo *philo);
void	precise_usleep(long usec, t_table *table);

/* SYNCHRO */
void	wait_all_threads(t_table *table);
bool	all_threads_running(t_mutex *mutex, long *threads, long philo_nb);
void	increase_long(t_mutex *mutex, long *value);

/* MONITOR */
void	*monitor(void *data);

/* PARSING UTILS */
long	ft_atol(char *nptr);

#endif