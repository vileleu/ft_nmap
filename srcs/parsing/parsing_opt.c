/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_opt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:48:39 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/24 01:00:39 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

uint8_t		get_opt_ip(t_parse *parse) {
	t_get_arg	tmp;

	tmp.list = NULL;
	bzero(&tmp, sizeof(t_get_arg));
	if ((parse->skip_next && (!parse->next || !*parse->next)) || (!parse->skip_next && !(*(parse->actual))))
		return (error_parsing(parse, "ip option need 1 argument", parse->i));
	if (parse->skip_next) {
		if (get_string(&tmp, parse->next, ',')) {
			free_list(tmp.list);
			return (error_parsing(parse, "error malloc during ip option", parse->i));
		}
	}
	else {
		if (get_string(&tmp, parse->actual, ',')) {
			free_list(tmp.list);
			return (error_parsing(parse, "error malloc during ip option", parse->i));
		}
	}
	if (parse->opt->targets)
		last_list(parse->opt->targets)->next = tmp.list;
	else
		parse->opt->targets = tmp.list;
	return (EXIT_SUCCESS);
}

uint8_t		get_opt_port(t_parse *parse) {
	t_get_arg	tmp;

	bzero(&tmp, sizeof(t_get_arg));
	if ((parse->skip_next && (!parse->next || !*parse->next)) || (!parse->skip_next && !(*(parse->actual))))
		return (error_parsing(parse, "port option need 1 argument", parse->i));
	if (parse->port_ok)
		return (error_parsing(parse, "1 option port is allowed", parse->i));
	if (get_number(parse, &tmp, 1))
		return (EXIT_FAILURE);
	if (parse->is_ranged) {
		if (tmp.ranged.max > MAX_PORT || tmp.ranged.min > tmp.ranged.max)
			return (error_parsing_example(parse, "port range is wrong", "(range:0/65535)", parse->i + parse->skip_next));
		parse->opt->port.min = tmp.ranged.min;
		parse->opt->port.max = tmp.ranged.max;
	}
	else {
		if (tmp.nb > MAX_PORT)
				return (error_parsing_example(parse, "port number is wrong", "(n <= 65535)", parse->i + parse->skip_next));
		parse->opt->port.isranged = 0;
		parse->opt->port.port = tmp.nb;
	}
	parse->port_ok = 1;
	return (EXIT_SUCCESS);
}

uint8_t		get_opt_thread(t_parse *parse) {
	t_get_arg	tmp;

	bzero(&tmp, sizeof(t_get_arg));
	if ((parse->skip_next && (!parse->next || !*parse->next)) || (!parse->skip_next && !(*(parse->actual))))
		return (error_parsing(parse, "speedup option need 1 argument", parse->i));
	if (parse->thread_ok)
		return (error_parsing(parse, "1 option speedup is allowed", parse->i));
	if (get_number(parse, &tmp, 0))
		return (EXIT_FAILURE);
	if (tmp.nb < 0 || tmp.nb > MAX_THREAD)
		return (error_parsing_example(parse, "speedup number is wrong", "(0 <= n <= 250)", parse->i + parse->skip_next));
	parse->opt->thread = tmp.nb;
	parse->thread_ok = 1;
	return (EXIT_SUCCESS);
}
