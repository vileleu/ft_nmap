/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 01:03:04 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/25 23:14:52 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

uint8_t		get_string(t_get_arg *tmp, const char *s, const char c) {
	char		*data = NULL;
	uint32_t	i = 0;

	while (s[i] && s[i] != c)
		i++;
	if (i) {
		if (!(data = strndup(s, i)))
			return (EXIT_FAILURE);
		if (add_list(&tmp->list, data))
			return (EXIT_FAILURE);
	}
	if (s[i])
		return (get_string(tmp, s + i + 1, c));
	return (EXIT_SUCCESS);
}

uint8_t		get_number(t_parse *parse, t_get_arg *tmp, const uint8_t canbe_ranged) {
	if (canbe_ranged) {
		if (!parse->skip_next && str_isranged(parse->actual)) {
			parse->is_ranged = 1;
			tmp->ranged = atoi_ranged(parse->actual);
			return (EXIT_SUCCESS);
		}
		else if (parse->skip_next && str_isranged(parse->next)) {
			parse->is_ranged = 1;
			tmp->ranged = atoi_ranged(parse->next);
			return (EXIT_SUCCESS);
		}
	}
	if (!parse->skip_next && str_isdigit(parse->actual))
		tmp->nb = atoi(parse->actual);
	else if (parse->skip_next && str_isdigit(parse->next))
		tmp->nb = atoi(parse->next);
	else
		return (error_parsing(parse, "wrong format", parse->i + parse->skip_next));
	return (EXIT_SUCCESS);
}