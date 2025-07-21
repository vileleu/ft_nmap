/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:57 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/22 00:51:42 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NMAP_H
#define FT_NMAP_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_PORT_CLOSED_PRINT 25
#define MAX_PORT_SCAN 1024

typedef enum	e_scan {
	ALL, SYN, NUL, ACK, FIN, XMAS, UDP
}				t_scan;

typedef struct	s_ranged {
	int			min;
	int			max;
}				t_ranged;

typedef struct	s_ports {
	uint16_t	min;
	uint16_t	max;
}				t_ports;

typedef struct	s_opt {
	char		**targets;
	uint32_t	len_targets;
	uint8_t		check_ports;
	uint8_t		ports_isranged;
	uint16_t	ports;
	t_ports		ports_ranged;
	uint8_t		check_threads;
	uint8_t		threads;
	uint8_t		check_scan;
	t_scan		scan;
}				t_opt;

typedef struct	s_parse {
	char		**arg;
	char		*actual;
	char		*next;
	uint8_t		i;
	uint8_t		shortopt;
	t_ranged	tmp_ranged;
	int			tmp_nb;
	uint8_t		is_ranged;
	uint8_t		skip_next;
	t_opt		opt;
}				t_parse;

/*
** ERROR FUNCTIONS
*/

uint8_t			error_parsing_malloc(t_parse *parse);
uint8_t			error_parsing_number(t_parse *parse, const char *msg, uint8_t i);
uint8_t			error_parsing_unknown(t_parse *parse, uint8_t i);

/*
** PARSING FUNCTIONS
*/

t_ranged		atoi_ranged(const char *s);
uint8_t			str_isranged(const char *s);
uint8_t			str_isdigit(const char *s);

#endif