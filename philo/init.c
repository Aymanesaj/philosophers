/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:08:05 by asajed            #+#    #+#             */
/*   Updated: 2025/05/06 20:09:41 by asajed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lock_forks(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(philo->f_fork);
	print_state("has taken a fork", philo, data);
	if (philo->f_fork == philo->s_fork)
	{
		pthread_mutex_unlock(philo->f_fork);
		return (1);
	}
	pthread_mutex_lock(philo->s_fork);
	print_state("has taken a fork", philo, data);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (!data->stop)
	{
		if (lock_forks(data, philo))
			return (NULL);
		pthread_mutex_lock(&data->mtx_monitor);
		philo->last_meal = get_current_time(data);
		philo->meals_eaten++;
		pthread_mutex_unlock(&data->mtx_monitor);
		print_state("is eating", philo, data);
		ft_usleep(data->time_to_eat, data);
		pthread_mutex_unlock(philo->f_fork);
		pthread_mutex_unlock(philo->s_fork);
		print_state("is sleeping", philo, data);
		ft_usleep(data->time_to_sleep, data);
		print_state("is thinking", philo, data);
	}
	return (arg);
}

int	check_philo_state(t_philo *philo, t_data *data)
{
	ssize_t	time;

	int (i), (j);
	i = 0;
	j = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->mtx_monitor);
		time = get_current_time(data);
		if (time - philo[i].last_meal > data->time_to_die)
		{
			print_state("died", &philo[i], data);
			data->stop = true;
			pthread_mutex_unlock(&data->mtx_monitor);
			return (-1);
		}
		if (data->meals_max >= 0 && philo[i].meals_eaten >= data->meals_max)
			j++;
		pthread_mutex_unlock(&data->mtx_monitor);
		i++;
	}
	return (j);
}

void	*monitoring(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo[0].data;
	int (j);
	while (!data->stop)
	{
		j = check_philo_state(philo, data);
		if (j == -1)
			return (NULL);
		if (j == data->philo_count)
		{
			data->stop = true;
			return (NULL);
		}
		usleep(10);
	}
	return (NULL);
}

void	init_data(t_data *data, t_philo *philo)
{
	int (i);
	i = 0;
	data->start_time = get_current_time(NULL);
	data->threads = malloc(sizeof(pthread_t) * data->philo_count);
	while (i < data->philo_count)
	{
		memset(&philo[i], 0, sizeof(t_philo));
		philo[i].id = i + 1;
		philo[i].f_fork = &data->fork[i];
		philo[i].s_fork = &data->fork[(i + 1) % data->philo_count];
		if (philo[i].id % 2 == 0)
		{
			philo[i].f_fork = philo[i].s_fork;
			philo[i].s_fork = &data->fork[i];
		}
		philo[i].data = data;
		pthread_create(&philo[i].philo, NULL, routine, &philo[i]);
		data->threads[i] = philo[i].philo;
		i++;
	}
	pthread_create(&data->monitor, NULL, monitoring, philo);
	i = 0;
	while (i < data->philo_count)
		pthread_join(data->threads[i++], NULL);
	pthread_join(data->monitor, NULL);
}
