/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marguima <marguima@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 20:47:45 by marguima          #+#    #+#             */
/*   Updated: 2026/01/25 15:08:17 by marguima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Assigne les fourchettes adjacentes à un philosophe 
 * avec prévention de deadlock.
 * 
 * @details Pour éviter un deadlock, les philosophes pairs et impairs
 * prennent leurs fourchettes dans un ordre différent :
 * Philosophes impairs: prennent d'abord la fourchette gauche, puis la droite
 * Philosophes pairs: prennent d'abord la fourchette droite, puis la gauche
 * Cette asymétrie garantit qu'au moins un philosophe peut toujours progresser
 * 
 * @param philo Pointeur vers le philosophe à initialiser
 * @param forks Tableau de toutes les fourchettes disponibles sur la table
 * @param philo_pos Position du philosophe dans le tableau (index 0-based)
 * 
 * @note La fourchette à droite est à l'index philo_pos
 * @note La fourchette à gauche est à l'index (philo_pos + 1) % philo_nbr.
 * (Le modulo fait boucler l'index pour que le dernier philosophe ait comme 
 * fourchette gauche la fourchette 0, créant ainsi la table circulaire)
 */
void	assign_fork(t_philo *philo, t_fork *forks, int philo_pos)
{
	int	philo_nbr;

	philo_nbr = philo->table->philo_nbr;
	philo->second_fork = &forks[philo_pos];
	philo->first_fork = &forks[(philo_pos + 1) % philo_nbr];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[philo_pos];
		philo->second_fork = &forks[(philo_pos + 1) % philo_nbr];
	}
}

/**
 * @brief Initialise chaque philosophe à i + 1 (Premier philo == 1 pas 0) et
 * assigne les forks initialisés auparavant à chacun des philos via assign_fork
 * .
 * 
 * @param table 
 */
void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (table->philo_nbr > ++i)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->full = false;
		philo->meals_counter = 0;
		philo->table = table;
		safe_handle_mutex(&philo->philo_mutex, INIT);
		assign_fork(philo, table->forks, i);
	}
}

/**
 * @brief Initialise le reste de la structure table
 * en allouant un tableau de philosophes et de forks.
 * Forks : INIT les mutex pour chaque fouchette du tableau
 * et leur assigne leur ID basé sur le nb de philosophes.
 * Philos : Appelle la fonction philo_init pour l'initialisation des philos.
 * 
 * @param table 
 */
void	data_init(t_table *table)
{
	int	i;

	i = -1;
	table->end_simulation = false;
	table->all_ready = false;
	table->philos = safe_malloc(sizeof(t_philo) * table->philo_nbr);
	table->threads_running_nb = 0;
	safe_handle_mutex(&table->table_mutex, INIT);
	safe_handle_mutex(&table->write_lock, INIT);
	table->forks = safe_malloc(sizeof(t_fork) * table->philo_nbr);
	while (table->philo_nbr > ++i)
	{
		safe_handle_mutex(&table->forks[i].fork, INIT);
		table->forks[i].fork_id = i;
	}
	philo_init(table);
}
