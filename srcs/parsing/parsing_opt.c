/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_opt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:48:39 by vileleu           #+#    #+#             */
/*   Updated: 2025/10/16 17:50:19 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

uint8_t		get_opt_ip(t_parse *parse) {
	const char	*s = (parse->skip_next ? parse->next : parse->actual);
	t_get_arg	tmp;

	tmp.un.list_addr = NULL;
	bzero(&tmp, sizeof(t_get_arg));
	if ((parse->skip_next && (!parse->next || !*parse->next)) || (!parse->skip_next && !(*(parse->actual))))
		return (error_parsing(parse, "ip option need 1 argument", parse->i));
	if (get_string(parse, &tmp, s)) {
			tmp.un.list_addr ? free_list_addr(tmp.un.list_addr) : (void)0;
			return (EXIT_FAILURE);
	}
	if (parse->opt->targets)
		last_list_addr(parse->opt->targets)->next = tmp.un.list_addr;
	else
		parse->opt->targets = tmp.un.list_addr;
	return (EXIT_SUCCESS);
}

uint8_t		get_opt_file(t_parse *parse) {
	const char	*s = (parse->skip_next ? parse->next : parse->actual);

	if ((parse->skip_next && (!parse->next || !*parse->next)) || (!parse->skip_next && !(*(parse->actual))))
		return (error_parsing(parse, "file option need 1 argument", parse->i));
	if (get_file(parse, s))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

uint8_t		get_opt_port(t_parse *parse) {
	t_get_arg	tmp;

	tmp.un.list = NULL;
	bzero(&tmp, sizeof(t_get_arg));
	if ((parse->skip_next && (!parse->next || !*parse->next)) || (!parse->skip_next && !(*(parse->actual))))
		return (error_parsing(parse, "port option need 1 argument", parse->i));
	if (parse->port_ok)
		return (error_parsing(parse, "only 1 option port is allowed", parse->i));
	if (get_number(parse, &tmp, 1, 1)) {
		tmp.un.list ? free_list(tmp.un.list) : (void)0;
		return (EXIT_FAILURE);
	}
	if (parse->is_ranged) {
		if (tmp.un.ranged.max > MAX_PORT || tmp.un.ranged.min > tmp.un.ranged.max || tmp.un.ranged.max - tmp.un.ranged.min >= MAX_RANGE_SCAN)
			return (error_parsing_example(parse, "port range is wrong", "(0 < port <= 65535, max range = 1024)", parse->i + parse->skip_next));
		parse->opt->port.min = tmp.un.ranged.min;
		parse->opt->port.max = tmp.un.ranged.max;
	}
	else if (parse->is_list) {
		if (tmp.error == ERROR_LIMIT || count_list(tmp.un.list) > MAX_RANGE_SCAN) {
			tmp.un.list ? free_list(tmp.un.list) : (void)0;
			return (error_parsing_example(parse, "port list is wrong", "(0 < port <= 65535, max range = 1024)", parse->i + parse->skip_next));
		}
		else if (tmp.error == ERROR_SAME) {
			tmp.un.list ? free_list(tmp.un.list) : (void)0;
			return (error_parsing(parse, "same port in list is not allowed", parse->i + parse->skip_next));
		}
		parse->opt->port.isranged = 0;
		parse->opt->port.islist = 1;
		parse->opt->port.list = tmp.un.list;
	}
	else {
		if (tmp.un.nb > MAX_PORT)
			return (error_parsing_example(parse, "port number is wrong", "(0 < port <= 65535)", parse->i + parse->skip_next));
		parse->opt->port.isranged = 0;
		parse->opt->port.port = tmp.un.nb;
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
		return (error_parsing(parse, "only 1 option speedup is allowed", parse->i));
	if (get_number(parse, &tmp, 0, 0))
		return (EXIT_FAILURE);
	if (tmp.un.nb < 0 || tmp.un.nb > MAX_THREAD)
		return (error_parsing_example(parse, "speedup number is wrong", "(0 <= n <= 250)", parse->i + parse->skip_next));
	parse->opt->thread = tmp.un.nb;
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
	while (i_scan < SIZE_SCAN && parse->opt->scan[i_scan])
		i_scan++;
	if ((ret_scan = get_scan(parse, s, i_scan)) == 1)
		return (error_parsing_example(parse, "bad argument for scan option", "(SYN,NULL,ACK,FIN,XMAS,UDP)", parse->i + parse->skip_next));
	return (EXIT_SUCCESS);
}

uint8_t		get_opt_help(const char **arg, const int len_arg) {
	int32_t	i = 0;

	while (i < len_arg) {
		if (!strcmp(arg[i], "--help")) {
			printf("Help Screen\nft_nmap [OPTIONS]\n");
			printf("--help Print this help screen\n");
			printf("--ports (-p) ports to scan (eg: 1 or 1-10 or 1,2,3)\n");
			printf("--ip (-i) ip addresse(s) to scan in dot format (eg: ip or ip1,ip2,ip3)\n");
			printf("--file (-f) File name containing IP addresses to scan\n");
			printf("--speedup (-sp) [250 max] number of parallel threads to use\n");
			printf("--scan (-sc) SYN/NULL/FIN/XMAS/ACK/UDP\n");
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
