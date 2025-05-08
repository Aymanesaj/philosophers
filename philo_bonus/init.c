/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:08:05 by asajed            #+#    #+#             */
/*   Updated: 2025/05/08 23:38:10 by asajed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo_state(t_data *data)
{
	ssize_t	time;

	int (j);
	j = 0;
	time = get_current_time(data);
	if (time - data->last_meal > data->time_to_die)
	{
		sem_post(data->meal);
		sem_wait(data->death);
		print_state("died", data);
		sem_wait(data->meal);
		while (j < data->philo_count)
		{
			sem_post(data->terminate);
			sem_post(data->done);
			j++;
		}
		data->stop = true;
		sem_post(data->meal);
		sem_post(data->death);
		return (-1);
	}
	if (data->meals_max >= 0 && data->meals_eaten >= data->meals_max)
		j = 2;
	return (j);
}

void	*monitoring(void *arg)
{
	t_data	*data;

	int (j);
	data = (t_data *)arg;
	while (!check_state(data))
	{
		sem_wait(data->meal);
		j = check_philo_state(data);
		if (j == -1)
			return (NULL);
		if (j == 2)
		{
			data->stop = true;
			sem_post(data->meal);
			sem_post(data->done);
			return (NULL);
		}
		sem_post(data->meal);
		usleep(1);
	}
	return (NULL);
}

int	check_state(t_data *data)
{
	sem_wait(data->meal);
	if (data->stop)
	{
		sem_post(data->meal);
		return (1);
	}
	sem_post(data->meal);
	return (0);
}

void	enter_room(t_data *data)
{
	sem_wait(data->room);
	sem_wait(data->forks);
	print_state("has taken a fork", data);
	sem_wait(data->forks);
	print_state("has taken a fork", data);
	sem_post(data->room);
	sem_wait(data->meal);
	data->last_meal = get_current_time(data);
	sem_post(data->meal);
}

void	philo_lifecycle(t_data *data)
{
	data->start_time = get_current_time(NULL);
	pthread_create(&data->monitor, NULL, monitoring, data);
	pthread_create(&data->signaler, NULL, signaling, data);
	while (!check_state(data))
	{
		enter_room(data);
		print_state("is eating", data);
		ft_usleep(data->time_to_eat, data);
		sem_wait(data->meal);
		data->meals_eaten++;
		sem_post(data->meal);
		sem_post(data->forks);
		sem_post(data->forks);
		print_state("is sleeping", data);
		ft_usleep(data->time_to_sleep, data);
		print_state("is thinking", data);
	}
	pthread_join(data->monitor, NULL);
	pthread_join(data->signaler, NULL);
	ft_exit(data);
}
