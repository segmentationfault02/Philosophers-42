/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marguima <marguima@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 11:49:00 by marguima          #+#    #+#             */
/*   Updated: 2026/01/25 15:04:55 by marguima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Attend que tous les threads soient créés et prêts.
 * 
 * Boucle active (spin wait) jusqu'à ce que le flag all_ready
 * soit mis à true. Permet de synchroniser le départ de tous
 * les philosophes au même moment.
 * 
 * @param table Pointeur vers la structure principale
 */
void	wait_all_threads(t_table *table)
{
	while (get_bool(&table->table_mutex, &table->all_ready) == false)
		;
}

/**
 * @brief Vérifie si tous les threads philosophes sont lancés.
 * 
 * Compare le nombre de threads actuellement en cours avec
 * le nombre total de philosophes de manière thread-safe.
 * 
 * @param mutex Mutex protégeant la variable threads
 * @param threads Pointeur vers le compteur de threads actifs
 * @param philo_nb Nombre total de philosophes
 * @return true si tous les threads sont lancés, false sinon
 */
bool	all_threads_running(t_mutex *mutex, long *threads, long philo_nb)
{
	bool	ret;

	ret = false;
	safe_handle_mutex(mutex, LOCK);
	if (*threads == philo_nb)
		ret = true;
	safe_handle_mutex(mutex, UNLOCK);
	return (ret);
}

/**
 * @brief Incrémente une variable long de manière thread-safe.
 * 
 * Protège l'incrémentation avec un mutex pour éviter les
 * race conditions lors de l'accès concurrent.
 * 
 * @param mutex Mutex protégeant la variable
 * @param value Pointeur vers la variable à incrémenter
 */
void	increase_long(t_mutex *mutex, long *value)
{
	safe_handle_mutex(mutex, LOCK);
	(*value)++;
	safe_handle_mutex(mutex, UNLOCK);
}
