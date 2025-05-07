/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:57:39 by asajed            #+#    #+#             */
/*   Updated: 2025/05/08 00:05:40 by asajed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fork_philos(t_data *data)
{
	int	i;
	int	id;
	int	status;

	i = 0;
	id = 0;
	status = 0;
	while (i < data->philo_count)
	{
		data->id = i + 1;
		id = fork();
		if (id == -1)
			(perror("")), (exit(EXIT_FAILURE));
		if (!id)
			philo_lifecycle(data);
		i++;
	}
	while (wait(&status) > 0)
		;
}
	/* {
		if (WIFEXITED(status))
			if (WEXITSTATUS(status));
				kill_all_children();
	}*/

int	main(int ac, char **av)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (init_philo(ac, av, &data))
		return (1);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meal");
	data.forks = sem_open("/forks", O_CREAT, 0644, data.philo_count);
	data.print = sem_open("/print", O_CREAT, 0644, 1);
	data.meal = sem_open("/meal", O_CREAT, 0644, 1);
	fork_philos(&data);
	sem_close(data.forks);
	sem_close(data.print);
	sem_close(data.meal);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meal");
}
