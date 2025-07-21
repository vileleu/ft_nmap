/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 17:49:08 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/22 00:52:48 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

uint8_t		get_opt_number(t_parse *parse, uint8_t canbe_ranged) {
	if (canbe_ranged) {
		if (parse->actual && str_isranged(parse->actual)) {
			parse->is_ranged = 1;
			parse->tmp_ranged = atoi_ranged(parse->actual)
			return (EXIT_SUCCESS);
		}
		else if (parse->next && str_isranged(parse->next)) {
			parse->skip_next = 1;
			parse->is_ranged = 1;
			parse->tmp_ranged = atoi_ranged(parse->next)
			return (EXIT_SUCCESS);
		}
	}
	if (parse->actual && str_isdigit(parse->actual))
		parse->tmp = atoi(parse->actual);
	else if (parse->next && str_isdigit(parse->next)) {
		parse->skip_next = 1;
		parse->tmp = atoi(parse->next);
	}
	else {
		if (!parse->shortopt || !parse->actual)
			return (error_parsing_number(parse, parse->next, parse->i + 1));
		else
			return (error_parsing_number(parse, parse->arg[parse->i], parse->i));
	}
	return (EXIT_SUCCESS);
}

uint8_t		get_opt(t_parse *parse) {
	parse->skip_next = 1;
	// --help
	if (!strcmp(parse->actual, "help")) {
	}
	// --ports [ports]
	else if (!strcmp(parse->actual, "ports")) {
		if (!(get_opt_number(parse, 1)))
			return (EXIT_FAILURE);
		if (parse->is_ranged) {
			parse->ports_ranged.min = parse->tmp_ranged.min;
			parse->ports_ranged.max = parse->tmp_ranged.max;
		}
		else
			parse->ports = parse->tmp_nb;
	}
	// --ip [address/host] (handle multiple)
	else if (!strcmp(parse->actual, "ip")) {
		parse->target = parse->next;
	}
	// --speedup [nb threads]
	else if (!strcmp(parse->actual, "speedup")) {
		if (!(get_opt_number(parse, 0)))
			return (EXIT_FAILURE);
		parse->threads = parse->tmp_nb;
	}
	// --scan [type of scan]
	else if (!strcmp(parse->actual, "scan")) {
	}
	else
		return (error_parsing_unknown(parse, parse->i));
	return (EXIT_SUCCESS);
}

uint8_t			get_shortopt(t_parse *parse) {
	parse->shortopt = 1;
	// ( -p [ports] ) or ( -p[ports] )
	if (!strncmp(parse->actual, "p", 1)) {
		parse->actual += 1;
		if (!(get_opt_number(parse, 1)))
			return (EXIT_FAILURE);
		if (parse->is_ranged) {
			parse->opt.ports_isranged = 1;
			parse->opt.ports_ranged.min = parse->tmp_ranged.min;
			parse->opt.ports_ranged.max = parse->tmp_ranged.max;
		}
		else
			parse->opt.ports = parse->tmp_nb;
	}
	// ( -i [address/host] ) or ( -i[address/host] )
	else if (!strncmp(parse->actual, "i", 1)) {
		parse->actual += 1;
		parse->target = parse->actual + 1;
	}
	// ( -sp [nb threads] ) or ( -sp[nb threads] )
	else if (!strncmp(parse->actual, "sp", 2)) {
		parse->actual += 2;
	}
	// ( -sc [type of scan] ) or ( -sc[type of scan] )
	else if (!strncmp(parse->actual, "sc", 2)) {
		parse->actual += 2;
	}
	else
		return (error_parsing_unknown(parse, parse->i));
	return (EXIT_SUCCESS);
}

static t_parse	*init_parse(const char **arg, const int len_arg) {
	t_parse		*parse;

	if (!(parse = malloc(sizeof(t_parse))))
		return (NULL);
	bzero(parse, sizeof(t_parse));
	parse->arg = arg;
	parse->actual = NULL;
	parse->next = NULL;
	parse->target = NULL;
	return (parse);
}

uint8_t			parsing(char **arg, int len_arg) {
	t_parse		*parse;
	
	parse->i = 1;
	if (!(parse = init_parse(arg[0])))
		return (error_parsing_malloc(parse));
	while (parse->i < len_arg) {
		parse->skip_next = 0;
		parse->is_ranged = 0;
		parse->shortopt = 0;
		// ( --option [argument] )
		if (!strncmp(arg[parse->i], "--", 2)) {
			parse->actual = arg[parse->i] + 2;
			parse->next = NULL;
			if (parse->i < len_arg)
				parse->next = arg[parse->i + 1];
			if (get_opt(parse))
				return (NULL);
		}
		// ( -option [argument] ) or ( -option[argument] )
		if (!strncmp(arg[parse->i], "-", 1)) {
			parse->actual = arg[parse->i] + 1;
			parse->next = NULL;
			if (parse->i < len_arg)
				parse->next = arg[parse->i + 1];
			if (get_shortopt(parse))
				return (NULL);
		}
		parse->i += parse->skip_next + 1;
	}
	return (&(parse->opt));
}