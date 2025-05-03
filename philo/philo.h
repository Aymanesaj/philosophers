/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:58:29 by asajed            #+#    #+#             */
/*   Updated: 2025/05/03 16:08:54 by asajed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	print;
	pthread_mutex_t	monitor;
	pthread_t		*threads;
	bool			stop;
	long			first_fork;
	long			second_fork;
	ssize_t			philo_count;
	ssize_t			time_to_die;
	ssize_t			time_to_eat;
	ssize_t			time_to_sleep;
	ssize_t			meals_max;
	ssize_t			start_time;
}					t_data;

typedef struct s_philo
{
	int				id;
	pthread_t		philo;
	ssize_t			meals_eaten;
	ssize_t			last_meal;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	t_data			*data;
}					t_philo;

int					init_philo(int ac, char **av, t_data *philo);
void				init_data(t_data *data, t_philo *philo);
int					ft_usleep(size_t milliseconds);
long				ft_atoi(char *arg);
void				print_state(char *msg, t_philo *philo, t_data *data);
ssize_t				get_current_time(t_data *philo);

#endif