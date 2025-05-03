/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:10:40 by asajed            #+#    #+#             */
/*   Updated: 2025/05/03 16:00:14 by asajed           ###   ########.fr       */
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

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time(NULL);
	while ((get_current_time(NULL) - start) < milliseconds)
		usleep(500);
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
		index++;
	}
	return (num);
}

void	print_state(char *msg, t_philo *philo, t_data *data)
{
	if (!data->stop)
	{
		pthread_mutex_lock(&data->print);
		if (!data->stop)
			printf("%zu %d %s\n", get_current_time(data), philo->id, msg);
		pthread_mutex_unlock(&data->print);
	}
}
