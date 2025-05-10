/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:08:05 by asajed            #+#    #+#             */
/*   Updated: 2025/05/10 20:23:35 by asajed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo_state(t_data *data)
{
	int (j);
	j = 0;
	if (get_current_time(data) - data->last_meal > data->time_to_die)
	{
		sem_wait(data->death);
		sem_wait(data->print);
		if (!check_state(data))
			printf("%zu %d died\n", get_current_time(data), data->id);
		while (j++ < data->philo_count)
		{
			sem_post(data->terminate);
			sem_post(data->done);
		}
		sem_wait(data->sig);
		data->stop = true;
		sem_post(data->sig);
		usleep(100);
		sem_post(data->death);
		sem_post(data->print);
		return (-1);
	}
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
		sem_post(data->meal);
		if (j == -1)
			return (NULL);
		if (j == 2)
		{
			sem_wait(data->sig);
			data->stop = true;
			sem_post(data->sig);
			sem_post(data->done);
			return (NULL);
		}
	}
	return (NULL);
}

int	check_state(t_data *data)
{
	sem_wait(data->sig);
	if (data->stop)
	{
		sem_post(data->sig);
		return (1);
	}
	sem_post(data->sig);
	return (0);
}

int	enter_room(t_data *data)
{
	sem_wait(data->room);
	sem_wait(data->forks);
	print_state("has taken a fork", data);
	if (data->philo_count == 1)
		return (1);
	sem_wait(data->forks);
	print_state("has taken a fork", data);
	sem_post(data->room);
	sem_wait(data->meal);
	data->last_meal = get_current_time(data);
	sem_post(data->meal);
	return (0);
}

void	philo_lifecycle(t_data *data)
{
	pthread_create(&data->monitor, NULL, monitoring, data);
	pthread_create(&data->signaler, NULL, signaling, data);
	while (!check_state(data))
	{
		if (enter_room(data))
			break ;
		print_state("is eating", data);
		ft_usleep(data->time_to_eat, data);
		data->meals_eaten++;
		if (data->meals_max >= 0 && data->meals_eaten >= data->meals_max)
		{
			sem_wait(data->sig);
			data->stop = true;
			sem_post(data->sig);
			sem_post(data->done);
		}
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
