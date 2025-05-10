/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:57:39 by asajed            #+#    #+#             */
/*   Updated: 2025/05/10 20:10:53 by asajed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*signaling(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	sem_wait(data->terminate);
	sem_wait(data->sig);
	data->stop = true;
	sem_post(data->sig);
	return (NULL);
}

void	catch_signals(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		sem_wait(data->done);
		i++;
	}
	while (i)
	{
		sem_post(data->terminate);
		i--;
	}
}

void	ft_exit(t_data *data)
{
	sem_close(data->forks);
	sem_close(data->print);
	sem_close(data->meal);
	sem_close(data->room);
	sem_close(data->done);
	sem_close(data->terminate);
	sem_close(data->death);
	sem_close(data->sig);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meal");
	sem_unlink("/room");
	sem_unlink("/term");
	sem_unlink("/done");
	sem_unlink("/death");
	sem_unlink("/sig");
	free(data->pids);
	exit(0);
}

void	fork_philos(t_data *data)
{
	int	i;
	int	id;
	int	status;

	i = 0;
	data->pids = malloc(data->philo_count * sizeof(pid_t));
	data->start_time = get_current_time(NULL);
	while (i < data->philo_count)
	{
		data->id = i + 1;
		data->pids[i] = fork();
		if (data->pids[i] == -1)
			(perror("")), (exit(EXIT_FAILURE));
		if (!data->pids[i])
			philo_lifecycle(data);
		i++;
	}
	catch_signals(data);
	i = -1;
	id = 1;
	while (id > 0)
		id = waitpid(-1, &status, 0);
}

int	main(int ac, char **av)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (init_philo(ac, av, &data))
		return (1);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meal");
	sem_unlink("/room");
	sem_unlink("/term");
	sem_unlink("/done");
	sem_unlink("/death");
	sem_unlink("/sig");
	data.forks = sem_open("/forks", O_CREAT, 0644, data.philo_count);
	data.room = sem_open("/room", O_CREAT, 0644, 1);
	data.print = sem_open("/print", O_CREAT, 0644, 1);
	data.meal = sem_open("/meal", O_CREAT, 0644, 1);
	data.terminate = sem_open("/term", O_CREAT, 0644, 0);
	data.done = sem_open("/done", O_CREAT, 0644, 0);
	data.death = sem_open("/death", O_CREAT, 0644, 1);
	data.sig = sem_open("/sig", O_CREAT, 0644, 1);
	fork_philos(&data);
	ft_exit(&data);
}
