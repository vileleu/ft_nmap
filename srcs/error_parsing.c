/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 22:34:30 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/22 00:52:08 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

t_opt	*error_parsing_malloc(t_parse *parse) {
	fprintf(stderr, "%s: error malloc\n", parse->arg[0]);
	free(parse);
	return (NULL);
}

uint8_t	error_parsing_number(t_parse *parse, const char *msg, uint8_t i) {
	fprintf(stderr, "%s: error '%s' at %d\n", parse->arg[0], msg, i);
	free(parse);
	return (EXIT_FAILURE);
}

uint8_t	error_parsing_unknown(t_parse *parse, uint8_t i) {
	fprintf(stderr, "%s: error unknown at %d\n", parse->arg[0], i);
	free(parse);
	return (EXIT_FAILURE);
}
