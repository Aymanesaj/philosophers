/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:08:05 by asajed            #+#    #+#             */
/*   Updated: 2025/05/03 16:11:00 by asajed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	deadlock_prevention()
// {
// 	if ()
// }

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (!data->stop)
	{
		if (philo->id % 2 == 1)
		{
			if (philo->l_fork != philo->r_fork)
				ft_usleep(100);
			else
				return (print_state("has taken a fork", philo, data), NULL);
		}
		pthread_mutex_lock(philo->l_fork);
		print_state("has taken a fork", philo, data);
		pthread_mutex_lock(philo->r_fork);
		print_state("has taken a fork", philo, data);
		pthread_mutex_lock(&data->monitor);
		philo->last_meal = get_current_time(data);
		philo->meals_eaten++;
		pthread_mutex_unlock(&data->monitor);
		print_state("is eating", philo, data);
		ft_usleep(data->time_to_eat);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		print_state("is sleeping", philo, data);
		ft_usleep(data->time_to_sleep);
		print_state("is thinking", philo, data);
	}
	return (arg);
}

void	*monitoring(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo[0].data;
	int (i), (j);
	while (!data->stop)
	{
		i = 0;
		j = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_lock(&data->monitor);
			if (data->meals_max >= 0 && philo[i].meals_eaten >= data->meals_max)
				j++;
			if (get_current_time(data) - philo[i].last_meal >= data->time_to_die)
			{
				print_state("died", &philo[i], data);
				data->stop = true;
			}
			if (j == data->philo_count)
				data->stop = true;
			i++;
			pthread_mutex_unlock(&data->monitor);
		}
		ft_usleep(1);
	}
	return (NULL);
}

void	init_data(t_data *data, t_philo *philo)
{
	pthread_t	monitor;

	int (i);
	i = 0;
	data->threads = malloc(sizeof(pthread_t) * data->philo_count);
	while (i < data->philo_count)
	{
		memset(&philo[i], 0, sizeof(t_philo));
		philo[i].id = i + 1;
		philo[i].l_fork = &data->fork[i];
		philo[i].r_fork = &data->fork[(i + 1) % data->philo_count];
		philo[i].data = data;
		pthread_create(&philo[i].philo, NULL, routine, &philo[i]);
		data->threads[i] = philo[i].philo;
		i++;
	}
	pthread_create(&monitor, NULL, monitoring, philo);
	i = 0;
	while (i < data->philo_count)
		pthread_join(data->threads[i++], NULL);
	pthread_join(monitor, NULL);
}

int	init_philo(int ac, char **av, t_data *philo)
{
	if (ac < 5 || ac > 6)
		return (printf("Error\n"), 1);
	philo->philo_count = ft_atoi(av[1]);
	if (philo->philo_count == -1)
		return (printf("Error\n"), 1);
	philo->time_to_die = ft_atoi(av[2]);
	if (philo->time_to_die == -1)
		return (printf("Error\n"), 1);
	philo->time_to_eat = ft_atoi(av[3]);
	if (philo->time_to_eat == -1)
		return (printf("Error\n"), 1);
	philo->time_to_sleep = ft_atoi(av[4]);
	if (philo->time_to_sleep == -1)
		return (printf("Error\n"), 1);
	philo->meals_max = ft_atoi(av[5]);
	if (philo->meals_max == -1 && av[5])
		return (printf("Error\n"), 1);
	philo->fork = malloc(sizeof(pthread_mutex_t) * philo->philo_count);
	if (!philo->fork)
		return (perror(""), 1);
	int (i);
	i = 0;
	while (i < philo->philo_count)
		pthread_mutex_init(&philo->fork[i++], NULL);
	return (0);
}
