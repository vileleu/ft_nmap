/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_opt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:48:39 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/27 23:25:43 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

uint8_t		get_opt_port(t_parse *parse) {
	t_get_arg	tmp;

	tmp.list = NULL;
	bzero(&tmp, sizeof(t_get_arg));
	if ((parse->skip_next && (!parse->next || !*parse->next)) || (!parse->skip_next && !(*(parse->actual))))
		return (error_parsing(parse, "port option need 1 argument", parse->i));
	if (parse->port_ok)
		return (error_parsing(parse, "only 1 option port is allowed", parse->i));
	if (get_number(parse, &tmp, 1, 1)) {
		tmp.list ? free_list(tmp.list) : (void)0;
		return (EXIT_FAILURE);
	}
	if (parse->is_ranged) {
		if (tmp.ranged.max > MAX_PORT || tmp.ranged.min > tmp.ranged.max)
			return (error_parsing_example(parse, "port range is wrong", "(range:0/65535)", parse->i + parse->skip_next));
		parse->opt->port.min = tmp.ranged.min;
		parse->opt->port.max = tmp.ranged.max;
	}
	else if (parse->is_list) {
		parse->opt->port.isranged = 0;
		parse->opt->port.islist = 1;
		parse->opt->port.list = tmp.list;
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

uint8_t		get_opt_ip(t_parse *parse) {
	const char	*s = (parse->skip_next ? parse->next : parse->actual);
	t_get_arg	tmp;

	tmp.list = NULL;
	bzero(&tmp, sizeof(t_get_arg));
	if ((parse->skip_next && (!parse->next || !*parse->next)) || (!parse->skip_next && !(*(parse->actual))))
		return (error_parsing(parse, "ip option need 1 argument", parse->i));
	if (get_string(parse, &tmp, s)) {
			tmp.list ? free_list(tmp.list) : (void)0;
			return (EXIT_FAILURE);
	}
	if (parse->opt->targets)
		last_list(parse->opt->targets)->next = tmp.list;
	else
		parse->opt->targets = tmp.list;
	return (EXIT_SUCCESS);
}

uint8_t		get_opt_thread(t_parse *parse) {
	t_get_arg	tmp;

	bzero(&tmp, sizeof(t_get_arg));
	if ((parse->skip_next && (!parse->next || !*parse->next)) || (!parse->skip_next && !(*(parse->actual))))
		return (error_parsing(parse, "speedup option need 1 argument", parse->i));
	if (parse->thread_ok)
		return (error_parsing(parse, "only 1 option speedup is allowed", parse->i));
	if (get_number(parse, &tmp, 0, 0))
		return (EXIT_FAILURE);
	if (tmp.nb < 0 || tmp.nb > MAX_THREAD)
		return (error_parsing_example(parse, "speedup number is wrong", "(0 <= n <= 250)", parse->i + parse->skip_next));
	parse->opt->thread = tmp.nb;
	parse->thread_ok = 1;
	return (EXIT_SUCCESS);
}

uint8_t		get_opt_scan(t_parse *parse) {
	const char	*s = (parse->skip_next ? parse->next : parse->actual);
	uint8_t		i_scan = 0;
	uint8_t		ret_scan = 0;

	if (!parse->scan_ok) {
		parse->scan_ok = 1;
		bzero((parse->opt->scan), sizeof(parse->opt->scan));
	}
	if ((parse->skip_next && (!parse->next || !*parse->next)) || (!parse->skip_next && !(*(parse->actual))))
		return (error_parsing(parse, "scan option need 1 argument", parse->i));
	while (i_scan < MAX_SIZE_SCAN && parse->opt->scan[i_scan])
		i_scan++;
	if ((ret_scan = get_scan(parse, s, i_scan)) == 1)
		return (error_parsing_example(parse, "bad argument for scan option", "(SYN,NULL,ACK,FIN,XMAS,UDP)", parse->i + parse->skip_next));
	return (EXIT_SUCCESS);
}
