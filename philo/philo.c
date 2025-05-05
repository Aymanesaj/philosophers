/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:57:39 by asajed            #+#    #+#             */
/*   Updated: 2025/05/05 12:13:41 by asajed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	philo = malloc(sizeof(t_philo) * data.philo_count);
	init_data(&data, philo);
	free(data.threads);
	free(philo);
	int (i);
	i = 0;
	while (i < data.philo_count)
		pthread_mutex_destroy(&data.fork[i++]);
	pthread_mutex_destroy(&data.print);
	free(data.fork);
}
