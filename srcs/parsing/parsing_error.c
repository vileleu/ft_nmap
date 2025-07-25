/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 22:34:30 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/25 23:15:42 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	*error_parsing_init(const char *name) {
	fprintf(stderr, "%s: error malloc\n", name + 2);
	return (NULL);
}

uint8_t	error_parsing_example(t_parse *parse, const char *msg, const char *example, const uint8_t i) {
	fprintf(stderr, "%s: error parsing: %s '%s' %s (arg %d)\n", parse->arg[0] + 2, msg, parse->arg[i], example, i);
	free_list(parse->opt->targets);
	free(parse->opt);
	free(parse);
	return (EXIT_FAILURE);
}

uint8_t	error_parsing(t_parse *parse, const char *msg, const uint8_t i) {
	fprintf(stderr, "%s: error parsing: %s '%s' (arg %d)\n", parse->arg[0] + 2, msg, parse->arg[i], i);
	free_list(parse->opt->targets);
	free(parse->opt);
	free(parse);
	return (EXIT_FAILURE);
}
