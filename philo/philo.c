/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:57:39 by asajed            #+#    #+#             */
/*   Updated: 2025/05/10 09:28:15 by asajed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_state(t_data *data)
{
	pthread_mutex_lock(&data->stop);
	if (data->stop_simulation)
	{
		pthread_mutex_unlock(&data->stop);
		return (1);
	}
	pthread_mutex_unlock(&data->stop);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	*philo;

	memset(&data, 0, sizeof(t_data));
	if (init_philo(ac, av, &data))
		return (1);
	if (!data.philo_count)
		return (0);
	pthread_mutex_init(&data.print, NULL);
	pthread_mutex_init(&data.mtx_monitor, NULL);
	pthread_mutex_init(&data.stop, NULL);
	philo = malloc(sizeof(t_philo) * data.philo_count);
	init_data(&data, philo);
	free(data.threads);
	free(philo);
	int (i);
	i = 0;
	while (i < data.philo_count)
		pthread_mutex_destroy(&data.fork[i++]);
	pthread_mutex_destroy(&data.print);
	pthread_mutex_destroy(&data.stop);
	pthread_mutex_destroy(&data.mtx_monitor);
	free(data.fork);
}
