/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:57 by vileleu           #+#    #+#             */
/*   Updated: 2025/09/08 17:29:04 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NMAP_H
#define FT_NMAP_H

#include <ctype.h>
#include <netdb.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <time.h>
#include <pthread.h>
#include <pcap.h>

#define SIZE_SCAN 6

#define MAX_PORT 65535
#define MAX_THREAD 250

#define MAX_RANGE_SCAN 1024
#define MIN_PORT_SOURCE 30000
#define MAX_PORT_SOURCE (MAX_PORT - MAX_RANGE_SCAN * SIZE_SCAN)

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
	uint8_t		scan[SIZE_SCAN];
}				t_opt;

typedef struct	s_port_status {
    uint8_t open:1;
    uint8_t closed:1;
    uint8_t filtered:1;
    uint8_t unfiltered:1;
    uint8_t open_filtered:1;
	uint8_t	reserved:3;
}				t_port_status;

typedef struct	s_final_status {
	uint16_t				port;
	uint8_t 				scan[SIZE_SCAN];
	t_port_status			status[SIZE_SCAN];
	uint8_t					conclusion;
	struct s_final_status	*next;
}				t_final_status;

typedef struct s_thread_data {
    struct sockaddr_in	addr;
	const char			*ip;
    t_list 				*ports;  // Liste de ports pour ce thread
    uint8_t 			scan[SIZE_SCAN]; // Types de scan activés
	uint16_t			source;
	uint16_t			count;
	t_final_status		*final_status;
} t_thread_data;

typedef struct	s_list_result {
	uint16_t				source;
	uint16_t				dest;
	uint8_t					scan;
	t_port_status			status;
	struct s_list_result	*next;
}				t_list_result;

typedef struct	s_pcap_data {
	t_list_result		*l_result;
	pcap_t				*handle;
	struct bpf_program	filter;
	uint8_t				filter_on;
	uint16_t			nb_packet;
	struct timeval		tv;
	int					fd;
}				t_pcap_data;

/*
** PARSING FUNCTIONS
*/

t_opt			*parsing(const char **arg, const int len_arg);

/*
** SCANS FUNCTIONS
*/

t_pcap_data		*init_pcap_data(const char *ip, t_list *ports, uint16_t source, uint16_t count, uint8_t scan[SIZE_SCAN]);
uint8_t			scan_send(struct sockaddr_in *dst, t_list *ports, uint16_t source, uint8_t scan[SIZE_SCAN]);
uint8_t			scan_receive(t_pcap_data *p_data, t_final_status *final_status);
void			*free_pcap_data(t_pcap_data *p_data);

uint8_t			get_total_scan(uint8_t scan[SIZE_SCAN]);

/*
** OPT UTILS FUNCTIONS
*/

void			free_list(t_list *list);
void			free_list_addr(t_list_addr *list);
void    		free_opt(t_opt *opt);
void			print_opt(t_opt *opt);

/*
** INIT SCAN FUNCTIONS
*/

void			init_scan_configuration(t_opt *opt, t_final_status *f_s);

/*
** CHECK HOST FUNCTIONS
*/

int				check_host_availability(const char *ip_str);
uint16_t		get_source_port();

/*
** STATUS FUNCTIONS
*/

uint8_t			create_final_status(t_final_status **final_status, t_opt *opt);
void			print_conclusion(t_final_status *final_status);
void			free_final_status(t_final_status *final_status);

/*
** ERROR FUNCTIONS
*/

uint8_t			error_all(const char *msg);
uint8_t			error_all_errno(const char *msg);

#endif
