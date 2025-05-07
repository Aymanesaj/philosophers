/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:08:05 by asajed            #+#    #+#             */
/*   Updated: 2025/05/08 00:04:03 by asajed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo_state(t_data *data)
{
	ssize_t	time;

	int (j);
	j = 0;
	sem_wait(data->meal);
	time = get_current_time(data);
	if (time - data->last_meal > data->time_to_die)
	{
		print_state("died", data);
		data->stop = true;
		data->exit = true;
		data->forks->__align = 1;
		sem_post(data->meal);
		return (-1);
	}
	if (data->meals_max >= 0 && data->meals_eaten >= data->meals_max)
		j = 2;
	sem_post(data->meal);
	return (j);
}

void	*monitoring(void *arg)
{
	t_data	*data;

	int (j);
	data = (t_data *)arg;
	while (!data->stop)
	{
		j = check_philo_state(data);
		if (j == -1)
			return (NULL);
		if (j == 2)
		{
			data->stop = true;
			data->forks->__align = 1;
			return (NULL);
		}
	}
	return (NULL);
}

void	philo_lifecycle(t_data *data)
{
	data->start_time = get_current_time(NULL);
	pthread_create(&data->monitor, NULL, monitoring, data);
	while (!data->stop && data->forks->__align != 1)
	{
		if (data->id % 2 == 0)
			usleep(100);
		sem_wait(data->forks);
		print_state("has taken a fork", data);
		sem_wait(data->forks);
		print_state("has taken a fork", data);
		sem_wait(data->meal);
		data->last_meal = get_current_time(data);
		data->meals_eaten++;
		sem_post(data->meal);
		print_state("is eating", data);
		ft_usleep(data->time_to_eat, data);
		sem_post(data->forks);
		sem_post(data->forks);
		print_state("is sleeping", data);
		ft_usleep(data->time_to_sleep, data);
		print_state("is thinking", data);
	}
	pthread_join(data->monitor, NULL);
	exit(data->exit);
}
