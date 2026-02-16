/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marguima <marguima@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 15:20:25 by marguima          #+#    #+#             */
/*   Updated: 2026/01/25 15:18:22 by marguima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Vérifie si un philosophe est mort de faim.
 * 
 * Compare le temps écoulé depuis le dernier repas avec le
 * time_to_die. Retourne false si le philosophe est rassasié.
 * 
 * @param philo Pointeur vers le philosophe à vérifier
 * @return true si le philosophe est mort, false sinon
 */
bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	t_to_die;

	if (get_bool(&philo->philo_mutex, &philo->full))
		return (false);
	elapsed = get_time(MILLISECOND)
		- get_long(&philo->philo_mutex, &philo->last_meal_time);
	t_to_die = philo->table->time_to_die / 1e3;
	if (elapsed > t_to_die)
		return (true);
	return (false);
}

/**
 * @brief Thread de surveillance qui vérifie si un philo est mort.
 * 
 * Attend que tous les threads soient lancés, puis vérifie en
 * boucle si un philosophe est mort. Met fin à la simulation
 * dès qu'un décès est détecté.
 * 
 * @param data Pointeur vers la structure t_table
 * @return NULL
 */
void	*monitor(void *data)
{
	int		i;
	t_table	*table;

	table = (t_table *)data;
	while (!all_threads_running(&table->table_mutex, &table->threads_running_nb,
			table->philo_nbr))
		;
	while (!simulation_finished(table))
	{
		i = -1;
		while (table->philo_nbr > ++i && !simulation_finished(table))
		{
			if (philo_died(table->philos + i))
			{
				set_bool(&table->table_mutex, &table->end_simulation, true);
				write_status(DIED, table->philos + i);
			}
		}
	}
	return (NULL);
}
