/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marguima <marguima@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:00:05 by marguima          #+#    #+#             */
/*   Updated: 2026/01/25 14:21:58 by marguima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Fonction utile pour la verification d'arguments avant la conversion.
 * Verifie si l'argument n'est pas vide.
 * Skip les espaces et les signes '+'.
 * EXIT avec un message d'erreur si le nombre est negatif.
 * Verifie chaque CHAR et retourne true or false selon si c'est un digit ou pas
 * 
 * @param nb L'argument a checker
 * @return true 
 * @return false 
 */
bool	is_valid_nbr(char *nb)
{
	int	i;

	i = 0;
	if (!nb || !nb[0])
		return (false);
	while (nb[i] == 32 || nb[i] == '\t')
		i++;
	if (nb[i] == '+')
		i++;
	else if (nb[i] == '-')
		error("Syntax error : Only positive numbers is accepted\n");
	while (nb[i])
	{
		if (!(nb[i] >= '0' && nb[i] <= '9'))
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Fonction de conversion ASCII to Long.
 * EXIT FAILURE si l'argument n'est pas valide ou > INT MAX.
 * 
 * @param nptr Argument a convertir en long
 * @return long 
 */
long	ft_atol(char *nptr)
{
	size_t	i;
	long	stock;

	i = 0;
	stock = 0;
	if (!is_valid_nbr(nptr))
		error("Syntax error : Expected numbers\n");
	while (nptr[i])
		stock = stock * 10 + (nptr[i++] - '0');
	if (stock > INT_MAX)
		error("Syntax error : Numbers can't be higher than INT MAX\n");
	return (stock);
}

/**
 * @brief Initialise les arguments dans la structure t_table.
 * Check les erreurs de syntaxe.
 * 
 * @param table Structure de donnee initialisee dans le main
 * @param av Arguments passes au programme
 */
void	parse_input(t_table *table, char **av)
{
	table->philo_nbr = ft_atol(av[1]);
	table->time_to_die = ft_atol(av[2]) * MS_TO_US;
	table->time_to_eat = ft_atol(av[3]) * MS_TO_US;
	table->time_to_sleep = ft_atol(av[4]) * MS_TO_US;
	if (table->philo_nbr <= 0)
		error("Syntax error : Expected minimum 1 philosopher to work\n");
	if (table->time_to_die < 6e4 || table->time_to_eat < 6e4
		|| table->time_to_sleep < 6e4)
		error("Syntax error : > 60ms expected for the program to work\n");
	if (av[5])
		table->nbr_limit_meals = ft_atol(av[5]);
	else
		table->nbr_limit_meals = -1;
}
