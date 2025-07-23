/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 01:03:04 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/24 01:03:41 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

uint8_t		get_string(/*t_parse *parse, t_get_arg *tmp*/) {
	
	return (EXIT_SUCCESS);
}

uint8_t		get_number(t_parse *parse, t_get_arg *tmp, uint8_t canbe_ranged) {
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