/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:10:40 by asajed            #+#    #+#             */
/*   Updated: 2025/05/10 09:37:08 by asajed           ###   ########.fr       */
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
	while (((get_current_time(data) - start) < milliseconds)
		&& !check_state(data))
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

void	print_state(char *msg, t_philo *philo, t_data *data)
{
	if (!check_state(data))
	{
		pthread_mutex_lock(&data->print);
		if (!check_state(data))
			printf("%zu %d %s\n", get_current_time(data), philo->id, msg);
		pthread_mutex_unlock(&data->print);
	}
}

int	init_philo(int ac, char **av, t_data *philo)
{
	if (ac < 5 || ac > 6)
		return (write(2, "Error\n", 6), 1);
	philo->philo_count = ft_atoi(av[1]);
	if (philo->philo_count <= 0)
		return (write(2, "Error\n", 6), 1);
	philo->time_to_die = ft_atoi(av[2]);
	if (philo->time_to_die == -1)
		return (write(2, "Error\n", 6), 1);
	philo->time_to_eat = ft_atoi(av[3]);
	if (philo->time_to_eat == -1)
		return (write(2, "Error\n", 6), 1);
	philo->time_to_sleep = ft_atoi(av[4]);
	if (philo->time_to_sleep == -1)
		return (write(2, "Error\n", 6), 1);
	philo->meals_max = ft_atoi(av[5]);
	if (philo->meals_max == -1 && av[5])
		return (write(2, "Error\n", 6), 1);
	philo->fork = malloc(sizeof(pthread_mutex_t) * philo->philo_count);
	if (!philo->fork)
		return (perror(""), 1);
	int (i);
	i = 0;
	while (i < philo->philo_count)
		pthread_mutex_init(&philo->fork[i++], NULL);
	return (0);
}
