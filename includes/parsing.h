/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 00:15:38 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/27 21:03:12 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include "ft_nmap.h"

#define MIN_PORT_DEFAULT 1
#define MAX_PORT_DEFAULT 65535
#define THREAD_DEFAULT 0

#define SEPARATOR_STR ','

typedef struct	s_ranged {
	uint32_t	min;
	uint32_t	max;
}				t_ranged;

typedef struct	s_get_arg {
	t_list		*list;
	t_ranged	ranged;
	int32_t		nb;
}				t_get_arg;

typedef struct	s_parse {
	t_opt		*opt;
	const char	**arg;
	const char	*actual;
	const char	*next;
	uint8_t		port_ok;
	uint8_t		thread_ok;
	uint8_t		scan_ok;
	uint8_t		i;
	uint8_t		is_ranged;
	uint8_t		skip_next;
}				t_parse;

/*
** ERROR FUNCTIONS
*/

void			*error_parsing_init(const char *name);
uint8_t			error_parsing_example(t_parse *parse, const char *msg, const char *example, const uint8_t i);
uint8_t			error_parsing(t_parse *parse, const char *msg, const uint8_t i);

/*
** PARSING FUNCTIONS
*/

uint8_t			get_opt_port(t_parse *parse);
uint8_t			get_opt_ip(t_parse *parse);
uint8_t			get_opt_thread(t_parse *parse);
uint8_t			get_opt_scan(t_parse *parse);

uint8_t			get_scan(t_parse *parse, const char *s, char c, uint8_t j);
uint8_t			get_string(t_get_arg *tmp, const char *s, const char c);
uint8_t			get_number(t_parse *parse, t_get_arg *tmp, const uint8_t canbe_ranged);

t_ranged		atoi_ranged(const char *s);
uint8_t			str_isranged(const char *s);
uint8_t			str_isdigit(const char *s);
uint8_t			same_scan(uint8_t *scan, uint8_t check);

#endif

