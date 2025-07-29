/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 00:15:38 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/28 23:20:11 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include "ft_nmap.h"

#define MIN_PORT_DEFAULT 1
#define MAX_PORT_DEFAULT 65535
#define THREAD_DEFAULT 0

#define SEPARATOR_STR ','

#define ERROR_LIMIT 1
#define ERROR_SAME 2

#define BUFFER_SIZE 256

typedef struct	s_ranged {
	uint32_t	min;
	uint32_t	max;
}				t_ranged;

typedef struct	s_get_arg {
	union	un {
		t_list		*list;
		t_list_addr	*list_addr;
		t_ranged	ranged;
		int32_t		nb;
	}		un;
	uint8_t		error;
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
	uint8_t		is_list;
	uint8_t		skip_next;
}				t_parse;

/*
** ERROR FUNCTIONS
*/

void			*error_parsing_init(const char *name);
uint8_t			error_parsing_example(t_parse *parse, const char *msg, const char *example, const uint8_t i);
uint8_t			error_parsing_host(t_parse *parse, const char *host, const char *msg, const uint8_t i);
uint8_t			error_parsing(t_parse *parse, const char *msg, const uint8_t i);

/*
** PARSING FUNCTIONS
*/

uint8_t			get_opt_port(t_parse *parse);
uint8_t			get_opt_ip(t_parse *parse);
uint8_t			get_opt_file(t_parse *parse);
uint8_t			get_opt_thread(t_parse *parse);
uint8_t			get_opt_scan(t_parse *parse);
uint8_t			get_opt_help(const char **arg, const int len_arg);

uint8_t			get_scan(t_parse *parse, const char *s, uint8_t j);
uint8_t			get_string(t_parse *parse, t_get_arg *tmp, const char *s);
uint8_t			get_number(t_parse *parse, t_get_arg *tmp, const uint8_t canbe_ranged, const uint8_t canbe_list);
uint8_t			get_file(t_parse *parse, const char *s);

t_list			*atoi_list(t_get_arg *tmp, const char *s);
uint8_t			str_islist_nb(const char *s);
t_list_addr		*add_list_addr(t_list_addr **list, char *data);
uint8_t			add_list(t_list **list, uint16_t *data);
uint8_t     	same_list_addr(t_list_addr *list, char *host);
uint8_t			same_list_nb(t_list *list, uint16_t nb);
t_list_addr		*last_list_addr(t_list_addr *list);
t_list			*last_list(t_list *list);

t_ranged		atoi_ranged(const char *s);
uint8_t			str_ishost(t_parse *parse, t_list_addr *list, char *host);
uint8_t			str_isranged(const char *s);
uint8_t			str_isdigit(const char *s);
uint8_t			same_scan(uint8_t *scan, uint8_t check);
char			*ft_strjoin(char *s1, char const *s2);

#endif

