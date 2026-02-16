/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marguima <marguima@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 11:27:11 by marguima          #+#    #+#             */
/*   Updated: 2026/01/25 15:08:32 by marguima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Module contenant des setters_getters pour eviter d'ecrire
LOCK et UNLOCK partout. */

/**
 * @brief Modifie une variable booléenne de manière thread-safe
 * 
 * @param mutex Pointeur vers le mutex protégeant la variable
 * @param dest Pointeur vers la variable booléenne à modifier
 * @param value Nouvelle valeur à assigner
 * 
 * @note Cette fonction protège contre les data races en garantissant
 *       qu'un seul thread à la fois peut modifier la variable
 */
void	set_bool(t_mutex *mutex, bool *dest, bool value)
{
	safe_handle_mutex(mutex, LOCK);
	*dest = value;
	safe_handle_mutex(mutex, UNLOCK);
}

/**
 * @brief Lit une variable booléenne de manière thread-safe
 * 
 * @param mutex Pointeur vers le mutex protégeant la variable
 * @param value Pointeur vers la variable booléenne à lire
 * @return La valeur booléenne lue de manière atomique
 * 
 * @note Le mutex empêche la lecture pendant qu'un autre thread écrit
 */
bool	get_bool(t_mutex *mutex, bool *value)
{
	bool	ret;

	safe_handle_mutex(mutex, LOCK);
	ret = *value;
	safe_handle_mutex(mutex, UNLOCK);
	return (ret);
}

/**
 * @brief Modifie une variable long de manière thread-safe
 * 
 * @param mutex Pointeur vers le mutex protégeant la variable
 * @param dest Pointeur vers la variable long à modifier
 * @param value Nouvelle valeur à assigner
 * 
 * @note Utilisé pour les timestamps, compteurs de repas, etc.
 */
void	set_long(t_mutex *mutex, long *dest, long value)
{
	safe_handle_mutex(mutex, LOCK);
	*dest = value;
	safe_handle_mutex(mutex, UNLOCK);
}

/**
 * @brief Lit une variable long de manière thread-safe
 * 
 * @param mutex Pointeur vers le mutex protégeant la variable
 * @param value Pointeur vers la variable long à lire
 * @return La valeur long lue de manière atomique
 * 
 * @note Essentiel pour lire les timestamps sans race condition
 */
long	get_long(t_mutex *mutex, long *value)
{
	long	ret;

	safe_handle_mutex(mutex, LOCK);
	ret = *value;
	safe_handle_mutex(mutex, UNLOCK);
	return (ret);
}

/**
 * @brief Vérifie si la simulation est terminée de manière thread-safe
 * 
 * Fonction utilitaire qui encapsule l'accès à end_simulation avec
 * le mutex de la table. Permet de vérifier proprement l'état de
 * la simulation depuis n'importe quel thread.
 * 
 * @param table Pointeur vers la structure principale de la simulation
 * @return true si la simulation est terminée, false sinon
 * 
 * @note Cette fonction est appelée fréquemment dans les boucles des threads
 * pour savoir s'ils doivent continuer ou s'arrêter
 */
bool	simulation_finished(t_table *table)
{
	return (get_bool(&table->table_mutex, &table->end_simulation));
}
