# Philosophers

<div align="center">
  <img width="200" height="200" alt="image" src="https://github.com/user-attachments/assets/63ee82cd-cdab-43ca-b1e2-a758fb174f7c" />

  <h3>Le Problème des Philosophes - Synchronisation et Multithreading</h3>
  <p><em>Projet réalisé dans le cadre du cursus de 42 Lausanne</em></p>

  ![42](https://img.shields.io/badge/42-Lausanne-000000?style=for-the-badge&logo=42&logoColor=white)
  ![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
  ![Score](https://img.shields.io/badge/Score-100%2F100-brightgreen?style=for-the-badge)
</div>

---

## Table des Matières

- [À Propos](#-à-propos)
- [Le Problème](#-le-problème)
- [Fonctionnalités](#-fonctionnalités)
- [Installation](#-installation)
- [Utilisation](#-utilisation)
- [Architecture du Projet](#-architecture-du-projet)
- [Implémentation Technique](#-implémentation-technique)
- [Tests](#-tests)
- [Ressources](#-ressources)
- [Auteur](#-auteur)

---

## À Propos

**Philosophers** est une implémentation du célèbre **Problème des Philosophes** (Dining Philosophers Problem), formulé par Edsger Dijkstra en 1965. Ce projet est une introduction aux fondamentaux du **multithreading** et de la **synchronisation de processus** en C, utilisant les **POSIX threads (pthreads)** et les **mutex**.

L'objectif est de résoudre les défis classiques de la programmation concurrente :
- Éviter les **deadlocks** (interblocages)
- Prévenir les **data races** (accès concurrents)
- Garantir que les philosophes mangent régulièrement pour éviter la famine

---

## Le Problème

Un certain nombre de philosophes sont assis autour d'une table ronde avec un bol de spaghetti au centre. Des fourchettes sont placées entre chaque paire de philosophes adjacents.

Chaque philosophe alterne entre trois états :

```
     🍝 MANGER
    ↗         ↘
PENSER  ←  DORMIR
```

- Pour **manger**, un philosophe doit prendre les deux fourchettes à côté de lui (gauche et droite)
- Après avoir **mangé**, il repose les fourchettes et commence à **dormir**
- Après avoir **dormi**, il se met à **penser** jusqu'à ce qu'il ait faim à nouveau
- Si un philosophe ne mange pas dans le temps imparti (`time_to_die`), il **meurt**

La simulation s'arrête quand :
- Un philosophe meurt de faim
- Tous les philosophes ont mangé le nombre de repas requis (optionnel)

---

## Fonctionnalités

### Gestion des Threads
- Chaque philosophe est représenté par un **thread indépendant**
- Un **thread moniteur** surveille en continu l'état de chaque philosophe
- Démarrage synchronisé de tous les threads via un flag `all_ready`

### Prévention des Deadlocks
- Stratégie asymétrique de prise des fourchettes :
  - Philosophes **pairs** : prennent d'abord la fourchette droite, puis la gauche
  - Philosophes **impairs** : prennent d'abord la fourchette gauche, puis la droite
- Anti-famine : délai de réflexion supplémentaire pour les philosophes impairs

### Synchronisation Thread-Safe
- Chaque fourchette est protégée par un **mutex**
- Getters et setters thread-safe pour toutes les données partagées
- Mutex d'écriture pour des sorties console atomiques
- Mutex par philosophe pour protéger ses données individuelles

### Gestion d'Erreurs
- Wrappers sécurisés pour `malloc`, `mutex` et `thread`
- Gestion détaillée des codes d'erreur `errno`
- Validation complète des arguments d'entrée

### Précision Temporelle
- Implémentation de `precise_usleep` avec attente active
- Conversion automatique millisecondes → microsecondes
- Timestamps précis pour le suivi des événements

---

## Installation

### Prérequis

- Système Unix (Linux ou macOS)
- Compilateur `cc` ou `gcc`
- `make`
- Bibliothèque POSIX threads (incluse par défaut sur la plupart des systèmes)

### Compilation

```bash
git clone https://github.com/[votre-username]/philo.git
cd philo/philo
make
```

### Commandes Make

| Commande | Description |
|----------|-------------|
| `make` | Compilation du projet |
| `make clean` | Suppression des fichiers objets |
| `make fclean` | Suppression des fichiers objets et de l'exécutable |
| `make re` | Recompilation complète |

---

## Utilisation

### Lancement

```bash
./philo nombre_de_philosophes temps_avant_mort temps_pour_manger temps_pour_dormir [nombre_de_repas]
```

### Arguments

| Argument | Description |
|----------|-------------|
| `nombre_de_philosophes` | Nombre de philosophes (et de fourchettes) à la table |
| `temps_avant_mort` | Temps en millisecondes avant qu'un philosophe meure sans manger |
| `temps_pour_manger` | Temps en millisecondes pour manger |
| `temps_pour_dormir` | Temps en millisecondes pour dormir |
| `nombre_de_repas` | *(Optionnel)* La simulation s'arrête quand tous les philosophes ont mangé ce nombre de repas |

> **Note :** Les temps doivent être supérieurs ou égaux à 60 ms. Tous les arguments doivent être des entiers positifs.

### Exemples

```bash
# 5 philosophes - personne ne devrait mourir
./philo 5 800 200 200

# 4 philosophes - personne ne devrait mourir (limite)
./philo 4 410 200 200

# 4 philosophes - un philosophe devrait mourir
./philo 4 310 200 100

# 5 philosophes - s'arrête après 7 repas chacun
./philo 5 800 200 200 7

# 1 seul philosophe - il mourra (impossible de manger avec 1 fourchette)
./philo 1 800 200 200

# 200 philosophes - test de performance
./philo 200 800 200 200
```

### Format de Sortie

Le programme affiche les actions des philosophes avec un horodatage en millisecondes :

```
timestamp_ms X has taken a fork
timestamp_ms X is eating
timestamp_ms X is sleeping
timestamp_ms X is thinking
timestamp_ms X died
```

Où `X` est le numéro du philosophe (à partir de 1) et `timestamp_ms` est le temps écoulé depuis le début de la simulation.

---

## Architecture du Projet

### Structure des Répertoires

```
philo/
├── Makefile                    # Configuration de build
├── main.c                      # Point d'entrée et boucle principale
├── includes/
│   └── philo.h                 # Header principal (structures, prototypes, enums)
├── src/
│   ├── parsing.c               # Validation et conversion des arguments
│   ├── init.c                  # Initialisation des données (table, philosophes, fourchettes)
│   ├── dinner.c                # Logique principale de la simulation (manger, dormir, penser)
│   ├── monitor.c               # Thread de surveillance (détection de mort)
│   ├── safe_functions.c        # Wrappers sécurisés (malloc, mutex, thread)
│   ├── getters_setters.c       # Accesseurs thread-safe (bool, long)
│   ├── utils.c                 # Utilitaires (temps, affichage, nettoyage)
│   └── synchro_utils.c         # Utilitaires de synchronisation
└── assets/
    └── score.png               # Capture d'écran du score
```

### Structures de Données

```c
// Fourchette - protégée par un mutex
typedef struct s_fork {
    t_mutex  fork;       // Mutex de la fourchette
    int      fork_id;    // Identifiant unique
} t_fork;

// Philosophe - un thread par philosophe
typedef struct s_philo {
    int        id;             // Numéro du philosophe (1 à N)
    long       meals_counter;  // Nombre de repas consommés
    bool       full;           // A atteint le nombre de repas requis
    long       last_meal_time; // Timestamp du dernier repas
    t_fork     *first_fork;    // Première fourchette (ordre asymétrique)
    t_fork     *second_fork;   // Deuxième fourchette
    pthread_t  thread_id;      // ID du thread
    t_mutex    philo_mutex;    // Mutex personnel
    t_table    *table;         // Référence à la table
} t_philo;

// Table - données partagées de la simulation
struct s_table {
    long       philo_nbr;          // Nombre de philosophes
    long       time_to_die;        // Temps avant mort (microsecondes)
    long       time_to_eat;        // Temps pour manger (microsecondes)
    long       time_to_sleep;      // Temps pour dormir (microsecondes)
    long       nbr_limit_meals;    // Limite de repas (-1 si pas de limite)
    long       start_simulation;   // Timestamp de début
    bool       end_simulation;     // Flag de fin
    bool       all_ready;          // Tous les threads sont prêts
    long       threads_running_nb; // Compteur de threads actifs
    pthread_t  monitor;            // Thread moniteur
    t_mutex    table_mutex;        // Mutex de la table
    t_mutex    write_lock;         // Mutex pour l'affichage
    t_fork     *forks;             // Tableau de fourchettes
    t_philo    *philos;            // Tableau de philosophes
};
```

### Pipeline d'Exécution

```
┌─────────────────┐
│   Parse Input   │ → Validation des arguments
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   Data Init     │ → Allocation et initialisation des structures
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Dinner Start   │ → Création des threads philosophes + moniteur
└────────┬────────┘
         │
         ▼
┌─────────────────────────────────────────────┐
│  Simulation (threads en parallèle)          │
│                                             │
│  Philosophe 1: manger → dormir → penser ... │
│  Philosophe 2: manger → dormir → penser ... │
│  Philosophe N: manger → dormir → penser ... │
│  Moniteur:     vérifier → vérifier → ...    │
└────────┬────────────────────────────────────┘
         │
         ▼
┌─────────────────┐
│     Clean       │ → Destruction des mutex et libération mémoire
└─────────────────┘
```

---

## Implémentation Technique

### Prévention des Deadlocks

Le deadlock classique survient quand tous les philosophes prennent leur fourchette gauche simultanément et attendent indéfiniment la fourchette droite.

**Solution : prise asymétrique des fourchettes**

```
Philosophe pair (id % 2 == 0) :   Droite → Gauche
Philosophe impair (id % 2 == 1) : Gauche → Droite
```

Cela garantit qu'au moins un philosophe pourra toujours prendre ses deux fourchettes, brisant ainsi le cycle d'attente.

### Anti-Famine

Un délai de réflexion supplémentaire est ajouté pour les philosophes impairs afin d'équilibrer la distribution des repas et éviter que certains philosophes monopolisent les fourchettes.

### Synchronisation au Démarrage

Tous les threads philosophes attendent un flag `all_ready` avant de commencer la simulation, assurant un départ simultané et des conditions équitables.

### Détection de Mort

Le thread moniteur parcourt continuellement tous les philosophes et vérifie si le temps écoulé depuis leur dernier repas dépasse `time_to_die`. En cas de mort, le flag `end_simulation` est activé et tous les threads se terminent proprement.

### Affichage Thread-Safe

Un mutex dédié (`write_lock`) protège les appels à `printf`, garantissant que les messages de statut ne se chevauchent jamais et que l'horodatage reste cohérent.

---

## Tests

### Cas de Test Recommandés

```bash
# Test basique - aucun mort attendu
./philo 5 800 200 200

# Test limite - personne ne devrait mourir
./philo 4 410 200 200

# Test de mort - un philosophe doit mourir
./philo 4 310 200 100

# Test avec limite de repas
./philo 5 800 200 200 7

# Test philosophe seul - doit mourir
./philo 1 800 200 200

# Test de performance - 200 philosophes
./philo 200 800 200 200

# Tests d'erreur - doivent être rejetés
./philo -5 800 200 200       # Nombre négatif
./philo 5 800 200             # Arguments manquants
./philo 0 800 200 200         # Zéro philosophe
./philo 5 800 200 200 0       # Zéro repas
```

### Points de Vérification

- Aucun philosophe ne meurt avec `./philo 5 800 200 200`
- La mort est détectée en moins de 10 ms après `time_to_die`
- Pas de data races avec `-fsanitize=thread`
- Pas de fuites mémoire (hors readline/system)
- Les timestamps ne se chevauchent pas
- Un seul message `died` est affiché maximum

---

## Ressources

### Documentation et Références

- [Dining Philosophers Problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem) - Description du problème original
- [POSIX Threads Programming - LLNL](https://hpc-tutorials.llnl.gov/posix/) - Tutoriel complet sur les pthreads
- [pthread_mutex_lock - man7](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html) - Documentation des mutex POSIX
- [gettimeofday - man7](https://man7.org/linux/man-pages/man2/gettimeofday.2.html) - Documentation de gettimeofday
- Dijkstra, E. W. (1965). *"Solution of a Problem in Concurrent Programming Control"*

### Utilisation de l'IA

Les outils d'IA (Claude) ont été utilisés dans ce projet pour :
- **Création du README** : ce fichier a été généré avec l'assistance de l'IA
- **Clarification de concepts** : compréhension des stratégies de prévention de deadlock et des concepts de synchronisation

La conception, les algorithmes et l'écriture du code ont été réalisés manuellement pour garantir une compréhension approfondie des concepts de multithreading.

---

## Auteur

<div align="center">

| **marguima** |
|:---:|
| Marcello Guimaraes |

<br>

**42 Lausanne - Janvier 2026**

<br>

</div>
