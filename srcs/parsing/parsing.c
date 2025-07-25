/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 17:49:08 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/24 01:04:23 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_parse	*init_parse(const char **arg, t_opt *opt) {
	t_parse		*parse;

	if (!(parse = malloc(sizeof(t_parse)))) {
		free(opt);
		return (NULL);
	}
	bzero(parse, sizeof(t_parse));
	parse->arg = arg;
	parse->opt = opt;
	parse->actual = NULL;
	parse->next = NULL;
	return (parse);
}

static t_opt	*init_opt() {
	t_opt		*opt;
	uint8_t		tmp[] = {SYN, NUL, ACK, FIN, XMAS, UDP};

	if (!(opt = malloc(sizeof(t_opt))))
		return (NULL);
	bzero(opt, sizeof(t_opt));
	opt->targets = NULL;
	opt->port.min = MIN_PORT_DEFAULT;
	opt->port.max = MAX_PORT_DEFAULT;
	opt->thread = THREAD_DEFAULT;
	opt->port.isranged = 1;
    memcpy(opt->scan, tmp, sizeof(opt->scan));
	return (opt);
}

static uint8_t	get_opt(t_parse *parse) {
	parse->skip_next = 1;
	// --help
	if (!strcmp(parse->actual, "help")) {
		return (EXIT_FAILURE);
	}
	// --ports [port/range]
	else if (!strcmp(parse->actual, "ports")) {
		if (get_opt_port(parse))
			return (EXIT_FAILURE);
	}
	// --ip [address/host]
	else if (!strcmp(parse->actual, "ip")) {
		 //printf("------> IP parsing = %s\n", parse->next);
		 //PROVISOIRE - juste pour les test
		char **new_targets = realloc(parse->opt->targets, sizeof(char*) * (parse->opt->len_targets + 1));
		if (!new_targets) {
			fprintf(stderr, "Memory allocation failed\n");
			return EXIT_FAILURE;
		}
		parse->opt->targets = new_targets;
		parse->opt->targets[parse->opt->len_targets] = strdup(parse->next);
		if (!parse->opt->targets[parse->opt->len_targets]) {
			fprintf(stderr, "Memory allocation failed\n");
			return EXIT_FAILURE;
		}
		parse->opt->len_targets++;
		// printf pour debug
		printf("------> IP parsing = %s\n", parse->next);
	}
	// --speedup [nb thread]
	else if (!strcmp(parse->actual, "speedup")) {
		if (get_opt_thread(parse))
			return (EXIT_FAILURE);
	}
	// --scan [type of scan]
	else if (!strcmp(parse->actual, "scan")) {
	}
	else
		return (error_parsing(parse, "unknown option", parse->i));
	return (EXIT_SUCCESS);
}

static uint8_t	get_shortopt(t_parse *parse) {
	// ( -p [port/range] ) or ( -p[port/range] )
	if (!strncmp(parse->actual, "p", 1)) {
		(*(parse->actual += 1)) ? (parse->skip_next = 0) : (parse->skip_next = 1);
		if (get_opt_port(parse))
			return (EXIT_FAILURE);
	}
	// ( -i [address/host] ) or ( -i[address/host] )
	else if (!strncmp(parse->actual, "i", 1)) {
		parse->actual += 1;
	}
	// ( -sp [nb thread] ) or ( -sp[nb thread] )
	else if (!strncmp(parse->actual, "sp", 2)) {
		(*(parse->actual += 2)) ? (parse->skip_next = 0) : (parse->skip_next = 1);
		if (get_opt_thread(parse))
			return (EXIT_FAILURE);
	}
	// ( -sc [type of scan] ) or ( -sc[type of scan] )
	else if (!strncmp(parse->actual, "sc", 2)) {
		parse->actual += 2;
	}
	else
		return (error_parsing(parse, "unknown option", parse->i));
	return (EXIT_SUCCESS);
}

t_opt			*parsing(const char **arg, const int len_arg) {
	t_opt		*opt;
	t_parse		*parse;
	
	if (!(opt = init_opt()))
		return (error_parsing_malloc(arg[0]));
	if (!(parse = init_parse(arg, opt)))
		return (error_parsing_malloc(arg[0]));
	parse->i = 1;
	while (parse->i < len_arg) {
		parse->skip_next = 0;
		parse->is_ranged = 0;
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
		else if (!strncmp(arg[parse->i], "-", 1)) {
			parse->actual = arg[parse->i] + 1;
			parse->next = NULL;
			if (parse->i < len_arg)
				parse->next = arg[parse->i + 1];
			if (get_shortopt(parse))
				return (NULL);
		}
		else {
			error_parsing(parse, "unknown option", parse->i);
			return (NULL);
		}
		parse->i += parse->skip_next + 1;
	}
	free(parse);
	return (opt);
}
