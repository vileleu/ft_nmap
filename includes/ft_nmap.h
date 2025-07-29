/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:57 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/28 22:53:01 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NMAP_H
#define FT_NMAP_H

#include <ctype.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <time.h>


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
	uint16_t		*data;
	struct s_list	*next;
}				t_list;

typedef struct	s_list_addr {
	struct sockaddr_in	addr;
	char				*data;
	struct s_list_addr	*next;
}				t_list_addr;

typedef struct	s_port {
	uint16_t	port;
	uint8_t		isranged;
	uint16_t	min;
	uint16_t	max;
	uint8_t		islist;	
	t_list		*list;
}				t_port;

typedef struct	s_opt {
	t_list_addr	*targets;
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
void			free_list_addr(t_list_addr *list);
void    		free_opt(t_opt *opt);
void			print_opt(t_opt *opt);

#endif
