/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marguima <marguima@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 11:53:29 by marguima          #+#    #+#             */
/*   Updated: 2026/01/27 08:40:34 by marguima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Print un message d'erreur et exit failure.
 * 
 * @param str Message d'erreur
 */
void	error(char *str)
{
	printf("%s", str);
	exit (EXIT_FAILURE);
}

/**
 * @brief Récupère le temps actuel dans l'unité spécifiée.
 * 
 * Utilise gettimeofday pour obtenir le temps système et le
 * convertit en secondes, millisecondes ou microsecondes.
 * 
 * @param timecode Unité de temps désirée (SECOND/MILLISECOND/
 * MICROSECOND)
 * @return Le temps actuel dans l'unité spécifiée
 */
long	get_time(t_time_code timecode)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error("Philo : Gettimeofday failed.\n");
	if (timecode == SECOND)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (timecode == MILLISECOND)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (timecode == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		error("Philo : Wront input to gettime\n");
	return (42);
}

/**
 * @brief Affiche le statut d'un philosophe de manière thread-safe.
 * 
 * Calcule le temps écoulé et affiche le message correspondant
 * au statut. Protégé par un mutex pour éviter l'entrelacement
 * des messages. Ne print pas si le philo est rassasié.
 * 
 * @param status Statut actuel du philosophe
 * @param philo Pointeur vers le philosophe
 */
void	write_status(t_philo_status status, t_philo *philo)
{
	long	elapsed;

	if (philo->full)
		return ;
	safe_handle_mutex(&philo->table->write_lock, LOCK);
	elapsed = get_time(MILLISECOND) - philo->table->start_simulation;
	if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
		&& !simulation_finished(philo->table))
		printf("%-6ld %d has taken a fork\n", elapsed, philo->id);
	else if (status == EATING && !simulation_finished(philo->table))
		printf("%-6ld %d is eating\n", elapsed, philo->id);
	else if (status == SLEEPING && !simulation_finished(philo->table))
		printf("%-6ld %d is sleeping\n", elapsed, philo->id);
	else if (status == THINKING && !simulation_finished(philo->table))
		printf("%-6ld %d is thinking\n", elapsed, philo->id);
	else if (status == DIED)
		printf("%-6ld %d died\n", elapsed, philo->id);
	safe_handle_mutex(&philo->table->write_lock, UNLOCK);
}

/**
 * @brief Dort pendant une durée précise en microsecondes.
 * 
 * Implémente un sleep précis en vérifiant continuellement le temps
 * écoulé. Utilise usleep pour les longues périodes et une boucle
 * active pour la fin afin de maximiser la précision.
 * S'arrête immédiatement si la simulation se termine.
 * 
 * @param usec Durée de sommeil en microsecondes
 * @param table Pointeur vers la structure principale
 */
void	precise_usleep(long usec, t_table *table)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = get_time(MICROSECOND);
	while (get_time(MICROSECOND) - start < usec)
	{
		if (simulation_finished(table))
			break ;
		elapsed = get_time(MICROSECOND) - start;
		remaining = usec - elapsed;
		if (remaining > 1e3)
			usleep(remaining / 2);
		else
			while (get_time(MICROSECOND) - start < usec)
				;
	}
}

/**
 * @brief Libère toutes les ressources allouées.
 * 
 * Détruit tous les mutex (philosophes, table, write_lock)
 * et libère la mémoire allouée pour les fourchettes et
 * les philosophes.
 * 
 * @param table Pointeur vers la structure principale
 */
void	clean(t_table *table)
{
	t_philo	*philo;
	int		i;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = table->philos + i;
		safe_handle_mutex(&philo->philo_mutex, DESTROY);
	}
	safe_handle_mutex(&table->write_lock, DESTROY);
	safe_handle_mutex(&table->table_mutex, DESTROY);
	free(table->forks);
	free(table->philos);
}
