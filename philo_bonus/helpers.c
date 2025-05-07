/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:10:40 by asajed            #+#    #+#             */
/*   Updated: 2025/05/07 23:14:16 by asajed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

ssize_t	get_current_time(t_data *philo)
{
	struct timeval	time;
	long			start_time;

	start_time = 0;
	if (philo)
		start_time = philo->start_time;
	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec * 1000 + time.tv_usec / 1000) - start_time);
}

int	ft_usleep(size_t milliseconds, t_data *data)
{
	size_t	start;

	start = get_current_time(data);
	while (((get_current_time(data) - start) < milliseconds) && !data->stop)
		usleep(50);
	return (0);
}

long	ft_atoi(char *arg)
{
	int		index;
	long	num;

	index = 0;
	num = 0;
	if (arg && arg[index] == '+')
		index++;
	if (!arg || !arg[index])
		return (-1);
	while (arg[index])
	{
		if (!(arg[index] >= '0' && arg[index] <= '9'))
			return (-1);
		num = (arg[index] - 48) + (num * 10);
		if (num < -2147483648 || num > 2147483647)
			return (-1);
		index++;
	}
	return (num);
}

void	print_state(char *msg, t_data *data)
{
	if (!data->stop)
	{
		sem_wait(data->print);
		if (!data->stop)
			printf("%zu %d %s\n", get_current_time(data), data->id, msg);
		sem_post(data->print);
	}
}

int	init_philo(int ac, char **av, t_data *philo)
{
	if (ac < 5 || ac > 6)
		return (printf("Error\n"), 1);
	philo->philo_count = ft_atoi(av[1]);
	if (philo->philo_count <= 0)
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
	return (0);
}
