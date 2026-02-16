/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marguima <marguima@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 20:55:11 by marguima          #+#    #+#             */
/*   Updated: 2026/01/25 13:39:35 by marguima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Fonction utile pour faire une malloc et le verifier directement.
 * 
 * @param bytes Nombre de bytes a allouer
 * @return void*
 */
void	*safe_malloc(size_t bytes)
{
	void	*mal;

	mal = malloc(bytes);
	if (mal == NULL)
		error("Philosopher : Malloc error\n");
	return (mal);
}

/**
 * @brief Gère les erreurs des opérations sur les mutex.
 * 
 * Vérifie le code de retour des fonctions pthread_mutex et
 * affiche un message d'erreur détaillé selon le code errno.
 * Exit le programme si une erreur est détectée.
 * 
 * @param status Code de retour de la fonction pthread_mutex
 * @param fcode Code de l'opération (LOCK/UNLOCK/INIT/DESTROY)
 */
void	handle_mutex_error(int status, t_fcode fcode)
{
	if (status == 0)
		return ;
	if (status == EINVAL && (fcode == LOCK || fcode == UNLOCK))
		error("The value specified by mutex is invalid.\n");
	else if (status == EINVAL && (fcode == INIT || fcode == DESTROY))
		error("The value specified by attr is invalid.\n");
	else if (status == EDEADLK)
		error("A deadlock would occur if the thread "
			"blocked waiting for mutex.\n");
	else if (status == EPERM)
		error("The current thread does not hold a lock on mutex.\n");
	else if (status == ENOMEM)
		error("The process cannot allocate enough "
			"memory to create another mutex.\n");
	else if (status == EBUSY)
		error("Mutex is locked.\n");
}

/**
 * @brief Wrapper sécurisé pour les opérations sur les mutex.
 * 
 * Encapsule les appels pthread_mutex_* et vérifie les erreurs.
 * Simplifie l'utilisation des mutex dans le code.
 * 
 * @param mutex Pointeur vers le mutex
 * @param fcode Opération à effectuer (LOCK/UNLOCK/INIT/DESTROY)
 * @return NULL
 */
void	*safe_handle_mutex(t_mutex *mutex, t_fcode fcode)
{
	if (fcode == LOCK)
		handle_mutex_error(pthread_mutex_lock(mutex), fcode);
	else if (fcode == UNLOCK)
		handle_mutex_error(pthread_mutex_unlock(mutex), fcode);
	else if (fcode == INIT)
		handle_mutex_error(pthread_mutex_init(mutex, NULL), fcode);
	else if (fcode == DESTROY)
		handle_mutex_error(pthread_mutex_destroy(mutex), fcode);
	else
		error("Wrong mutex code\n");
	return (NULL);
}

/**
 * @brief Gère les erreurs des opérations sur les threads.
 * 
 * Vérifie le code de retour des fonctions pthread et affiche
 * un message d'erreur détaillé selon le code errno.
 * Exit le programme si une erreur est détectée.
 * 
 * @param status Code de retour de la fonction pthread
 * @param fcode Code de l'opération (CREATE/JOIN/DETACH)
 */
void	handle_thread_error(int status, t_fcode fcode)
{
	if (status == 0)
		return ;
	if (status == EAGAIN)
		error("No resources to create another thread.\n");
	else if (status == EPERM)
		error("No permission to set the scheduling "
			"policy and parameters.\n");
	else if (status == EINVAL && fcode == CREATE)
		error("The value specified by attr is invalid.\n");
	else if (status == EINVAL && (fcode == JOIN || fcode == DETACH))
		error("The value specified by thread is not joinable.\n");
	else if (status == ESRCH)
		error("No thread could be found.\n");
	else if (status == EDEADLK)
		error("A deadlock was detected.\n");
}

/**
 * @brief Wrapper sécurisé pour les opérations sur les threads.
 * 
 * Encapsule les appels pthread_create, pthread_join et
 * pthread_detach avec vérification d'erreurs.
 * 
 * @param thread Pointeur vers l'identifiant du thread
 * @param foo Fonction à exécuter par le thread (pour CREATE)
 * @param arg Arguments de la fonction (pour CREATE)
 * @param fcode Opération (CREATE/JOIN/DETACH)
 */
void	safe_thread_handle(pthread_t *thread, void *(*foo)(void *),
		void *arg, t_fcode fcode)
{
	if (fcode == CREATE)
		handle_thread_error(pthread_create(thread, NULL, foo, arg), fcode);
	else if (fcode == JOIN)
		handle_thread_error(pthread_join(*thread, NULL), fcode);
	else if (fcode == DETACH)
		handle_thread_error(pthread_detach(*thread), fcode);
	else
		error("Wrong code for thread_handle : "
			"Use <CREATE> <JOIN> <DETACH>\n");
}
