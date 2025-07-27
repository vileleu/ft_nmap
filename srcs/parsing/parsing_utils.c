/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:44:56 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/27 21:02:31 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_ranged	atoi_ranged(const char *s) {
	size_t		i = 0;
	t_ranged	ranged = {0, 0};

	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
		i++;
	if (!isdigit(s[i]))
		ranged.min = MIN_PORT_DEFAULT;
	while (isdigit(s[i]))
		ranged.min = ranged.min * 10 + (s[i++] - '0');
	if (s[i] == '-')
		i++;
	if (!isdigit(s[i]))
		ranged.max = MAX_PORT_DEFAULT;
	while (isdigit(s[i]))
		ranged.max = ranged.max * 10 + (s[i++] - '0');
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
		i++;
	return (ranged);
}

uint8_t		str_isranged(const char *s) {
	size_t	i = 0;
	uint8_t	dash = 0;
	uint8_t	check = 0;
	
	if (!s || !s[i])
		return (0);
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
		i++;
	if (isdigit(s[i]))
		check = 1;
	while (isdigit(s[i]) || s[i] == '-') {
		if (s[i] == '-' && !dash)
			dash = 1;
		else if (s[i] == '-' && dash)
			return (0);
		i++;
	}
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
		i++;
	if (s[i] || !dash || !check)
		return (0);
	return (1);
}

uint8_t		str_isdigit(const char *s) {
	size_t	i = 0;
	uint8_t	check = 0;

	if (!s || !s[i])
		return (0);
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
		i++;
	if (s[i] == '-')
		i++;
	if (isdigit(s[i]))
		check = 1;
	while (isdigit(s[i]))
		i++;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
		i++;
	if (s[i] || !check)
		return (0);
	return (1);
}

uint8_t		same_scan(uint8_t *scan, uint8_t check) {
	uint8_t	i = 0;

	while (i < MAX_SIZE_SCAN) {
		if (scan[i] == check)
			return (1);
		i++;
	}
	return (0);
}
