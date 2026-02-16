/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marguima <marguima@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 14:00:48 by marguima          #+#    #+#             */
/*   Updated: 2026/01/27 13:07:40 by marguima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Routine pour le cas d'un seul philosophe.
 * 
 * Un philosophe seul prend une fourchette et attend de mourir
 * car il ne peut pas manger avec une seule fourchette.
 * 
 * @param arg Pointeur vers la structure t_philo
 * @return NULL
 */
void	*alone_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_nb);
	write_status(TAKE_FIRST_FORK, philo);
	while (!simulation_finished(philo->table))
		usleep(200);
	return (NULL);
}

/**
 * @brief Fait penser un philosophe pendant une durée calculée.
 *
 * Affiche le statut THINKING pour tous les philosophes.
 * Le precise_usleep additionnel est une strategie anti-famine.
 *
 * @details Avec un nombre pair de philos, l'alternance naturelle
 * suffit (pas de delai). Avec un nombre impaire, un philo risque
 * d'etre bloque en permanence, donc tous attendent un temps
 * calcule (time_to_eat * 2 - time_to_sleep) pour equilibrer.
 * On utilise philo_nbr et non philo->id car avec philo->id,
 * certains philos attendent trop longtemps et meurent sur des
 * timings serres (ex: 4 410 200 200).
 *
 * @param philo Pointeur vers le philosophe
 */
void	thinking(t_philo *philo)
{
	write_status(THINKING, philo);
	if (philo->table->philo_nbr % 2 == 0)
		return ;
	precise_usleep(philo->table->time_to_eat / 2, philo->table);
}

/**
 * @brief Fait manger un philosophe.
 * 
 * Prend les deux fourchettes, met à jour last_meal_time,
 * incrémente le compteur de repas, dort pendant time_to_eat,
 * puis relâche les fourchettes. Marque le philo comme rassasié
 * si la limite de repas est atteinte.
 * 
 * @param philo Pointeur vers le philosophe
 */
void	eat(t_philo *philo)
{
	safe_handle_mutex(&philo->first_fork->fork, LOCK);
	write_status(TAKE_FIRST_FORK, philo);
	safe_handle_mutex(&philo->second_fork->fork, LOCK);
	write_status(TAKE_SECOND_FORK, philo);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	philo->meals_counter++;
	write_status(EATING, philo);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->nbr_limit_meals > 0
		&& philo->meals_counter == philo->table->nbr_limit_meals)
		set_bool(&philo->philo_mutex, &philo->full, true);
	safe_handle_mutex(&philo->first_fork->fork, UNLOCK);
	safe_handle_mutex(&philo->second_fork->fork, UNLOCK);
}

/**
 * @brief Routine principale exécutée par chaque thread philosophe.
 * 
 * Attend que tous les threads soient prêts, puis entre dans
 * la boucle principale : manger, dormir, penser.
 * S'arrête si le philosophe est rassasié ou si la simulation
 * se termine.
 * 
 * @param data Pointeur vers la structure t_philo du philosophe
 * @return NULL
 */
void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_nb);
	if (philo->id % 2)
		precise_usleep(philo->table->time_to_eat / 2, philo->table);
	while (!simulation_finished(philo->table))
	{
		if (get_bool(&philo->philo_mutex, &philo->full))
			break ;
		eat(philo);
		write_status(SLEEPING, philo);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		thinking(philo);
	}
	return (NULL);
}

/**
 * @brief Lance la simulation des philosophes.
 * 
 * Crée un thread pour chaque philosophe, démarre la simulation,
 * puis attend que tous les threads se terminent.
 * Gère le cas particulier d'un seul philosophe.
 * 
 * @param table Pointeur vers la structure principale
 */
void	dinner_start(t_table *table)
{
	int	i;

	i = -1;
	if (table->nbr_limit_meals == 0)
		return ;
	else if (table->philo_nbr == 1)
		safe_thread_handle(&table->philos[0].thread_id, alone_philo,
			&table->philos[0], CREATE);
	else
	{
		while (table->philo_nbr > ++i)
			safe_thread_handle(&table->philos[i].thread_id, dinner_simulation,
				&table->philos[i], CREATE);
	}
	safe_thread_handle(&table->monitor, monitor, table, CREATE);
	table->start_simulation = get_time(MILLISECOND);
	set_bool(&table->table_mutex, &table->all_ready, true);
	i = -1;
	while (table->philo_nbr > ++i)
		safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&table->table_mutex, &table->end_simulation, true);
	safe_thread_handle(&table->monitor, NULL, NULL, JOIN);
}
