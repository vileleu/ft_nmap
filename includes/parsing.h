/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 00:15:38 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/24 01:04:32 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include "ft_nmap.h"

#define MIN_PORT_DEFAULT 1
#define MAX_PORT_DEFAULT 65535
#define THREAD_DEFAULT 0

typedef struct	s_ranged {
	uint32_t	min;
	uint32_t	max;
}				t_ranged;

typedef struct	s_get_arg {
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
	uint8_t		i;
	uint8_t		is_ranged;
	uint8_t		skip_next;
}				t_parse;

/*
** ERROR FUNCTIONS
*/

void			*error_parsing_malloc(const char *name);
uint8_t			error_parsing_example(t_parse *parse, const char *msg, const char *example, uint8_t i);
uint8_t			error_parsing(t_parse *parse, const char *msg, uint8_t i);

/*
** PARSING FUNCTIONS
*/

uint8_t			get_opt_port(t_parse *parse);
uint8_t			get_opt_thread(t_parse *parse);

uint8_t			get_number(t_parse *parse, t_get_arg *tmp, uint8_t canbe_ranged);

t_ranged		atoi_ranged(const char *s);
uint8_t			str_isranged(const char *s);
uint8_t			str_isdigit(const char *s);

#endif

