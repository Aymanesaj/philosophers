/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asajed <asajed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:58:29 by asajed            #+#    #+#             */
/*   Updated: 2025/05/08 23:25:09 by asajed           ###   ########.fr       */
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
# include <fcntl.h>
# include <wait.h>
# include <semaphore.h>
# include <signal.h>

typedef struct s_data
{
	int				id;
	pid_t			*pids;
	sem_t			*done;
	sem_t			*forks;
	sem_t			*print;
	sem_t			*room;
	sem_t			*terminate;
	sem_t			*meal;
	sem_t			*death;
	pthread_t		monitor;
	pthread_t		signaler;
	volatile bool	stop;
	ssize_t			philo_count;
	ssize_t			time_to_die;
	ssize_t			time_to_eat;
	ssize_t			time_to_sleep;
	ssize_t			meals_max;
	ssize_t			start_time;
	ssize_t			meals_eaten;
	ssize_t			last_meal;
}					t_data;

int					init_philo(int ac, char **av, t_data *philo);
int					ft_usleep(size_t milliseconds, t_data *data);
long				ft_atoi(char *arg);
void				print_state(char *msg, t_data *data);
ssize_t				get_current_time(t_data *philo);
void				philo_lifecycle(t_data *data);
int					check_state(t_data *data);
void				ft_exit(t_data *data);
void				*signaling(void *arg);

#endif
