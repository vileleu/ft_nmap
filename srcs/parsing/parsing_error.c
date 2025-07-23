/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 22:34:30 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/24 00:23:43 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	*error_parsing_malloc(const char *name) {
	fprintf(stderr, "%s: error malloc\n", name + 2);
	return (NULL);
}

uint8_t	error_parsing_example(t_parse *parse, const char *msg, const char *example, uint8_t i) {
	fprintf(stderr, "%s: error parsing: %s '%s' %s (arg %d)\n", parse->arg[0] + 2, msg, parse->arg[i], example, i);
	free(parse->opt);
	free(parse);
	return (EXIT_FAILURE);
}

uint8_t	error_parsing(t_parse *parse, const char *msg, uint8_t i) {
	fprintf(stderr, "%s: error parsing: %s '%s' (arg %d)\n", parse->arg[0] + 2, msg, parse->arg[i], i);
	free(parse->opt);
	free(parse);
	return (EXIT_FAILURE);
}
