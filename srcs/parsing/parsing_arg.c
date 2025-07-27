/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 01:03:04 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/27 23:08:52 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

uint8_t		get_scan(t_parse *parse, const char *s, uint8_t j) {
	int32_t	i = 0;
	
	while (s[i] && s[i] != SEPARATOR_STR)
		i++;
	if (i) {
		if (i == 3 && !strncmp(s, "SYN", 3))
			(!same_scan(parse->opt->scan, SYN) ? (parse->opt->scan[j++] = SYN) : (void)0);
		else if (i == 4 && !strncmp(s, "NULL", 4))
			(!same_scan(parse->opt->scan, NUL) ? (parse->opt->scan[j++] = NUL) : (void)0);
		else if (i == 3 && !strncmp(s, "ACK", 3))
			(!same_scan(parse->opt->scan, ACK) ? (parse->opt->scan[j++] = ACK) : (void)0);
		else if (i == 3 && !strncmp(s, "FIN", 3))
			(!same_scan(parse->opt->scan, FIN) ? (parse->opt->scan[j++] = FIN) : (void)0);
		else if (i == 4 && !strncmp(s, "XMAS", 4))
			(!same_scan(parse->opt->scan, XMAS) ? (parse->opt->scan[j++] = XMAS) : (void)0);
		else if (i == 3 && !strncmp(s, "UDP", 3))
			(!same_scan(parse->opt->scan, UDP) ? (parse->opt->scan[j++] = UDP) : (void)0);
		else
			return (EXIT_FAILURE);
	}
	if (s[i])
		return (get_scan(parse, s + i + 1, j));
	return (EXIT_SUCCESS);
}

uint8_t		get_string(t_parse *parse, t_get_arg *tmp, const char *s) {
	char		*data = NULL;
	uint32_t	i = 0;

	while (s[i] && s[i] != SEPARATOR_STR)
		i++;
	if (i) {
		if (!(data = strndup(s, i)))
			return (error_parsing(parse, "error malloc while get string", parse->i + parse->skip_next));
		if (add_list(&tmp->list, data))
			return (error_parsing(parse, "error malloc while get string", parse->i + parse->skip_next));
	}
	if (s[i])
		return (get_string(parse, tmp, s + i + 1));
	return (EXIT_SUCCESS);
}

uint8_t		get_number(t_parse *parse, t_get_arg *tmp, const uint8_t canbe_ranged, const uint8_t canbe_list) {
	const char	*s = (parse->skip_next ? parse->next : parse->actual);

	if (canbe_ranged && str_isranged(s)) {
		parse->is_ranged = 1;
		tmp->ranged = atoi_ranged(s);
		return (EXIT_SUCCESS);
	}
	else if (canbe_list && str_islist(s)) {
		parse->is_list = 1;
		if (!(atoi_list(tmp, s)))
			return (error_parsing(parse, "error malloc while get number", parse->i + parse->skip_next));
		return (EXIT_SUCCESS);
	}
	if (str_isdigit(s))
		tmp->nb = atoi(s);
	else
		return (error_parsing(parse, "wrong format", parse->i + parse->skip_next));
	return (EXIT_SUCCESS);
}