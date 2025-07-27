/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:57 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/27 22:24:40 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NMAP_H
#define FT_NMAP_H

#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_RANGE_SCAN 1024

#define MAX_PORT 65535
#define MAX_THREAD 250

#define MAX_SIZE_SCAN 6

#define SYN 1
#define NUL 2
#define ACK 3
#define FIN 4
#define XMAS 5
#define UDP 6

typedef struct	s_list {
	void			*data;
	struct s_list	*next;
}				t_list;

typedef struct	s_port {
	uint16_t	port;
	uint8_t		isranged;
	uint16_t	min;
	uint16_t	max;
	uint8_t		islist;	
	t_list		*list;
}				t_port;

typedef struct	s_opt {
	t_list		*targets;
	t_port		port;
	uint8_t		thread;
	uint8_t		scan[6];
}				t_opt;

/*
** PARSING FUNCTIONS
*/

t_opt			*parsing(const char **arg, const int len_arg);

/*
** OPT UTILS FUNCTIONS
*/

void			free_list(t_list *list);
uint8_t			add_list(t_list **list, void *data);
t_list			*last_list(t_list *list);
void    		free_opt(t_opt *opt);
void			print_opt(const char *name, t_opt *opt);

#endif
