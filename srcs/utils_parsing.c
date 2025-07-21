/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:44:56 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/22 00:25:19 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

t_ranged	atoi_ranged(const char *s) {
	size_t		len = strlen(s);
	size_t		i = 0;
	t_ranged	ranged = {0, 0};

	while (i < len) {
		while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
			i++;
		while (isdigit(s[i++]))
			ranged.min = nb * 10 + (s[i] - '0');
		if (s[i] == '-')
			i++;
		while (isdigit(s[i++]))
			ranged.max = nb * 10 + (s[i] - '0');
		while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
			i++;
	}
	return (ranged);
}

uint8_t		str_isranged(const char *s) {
	size_t	len = strlen(s);
	size_t	i = 0;
	uint8_t	dash = 0;
	
	while (i < len) {
		if (s[i] == '-' && !dash)
			dash = 1;
		else if (s[i] == '-' && dash)
			return (0);
		if (!is_digit(s[i]))
			return (0);
	}
	if (dash)
		return (1);
	return (0);
}

uint8_t		str_isdigit(const char *s) {
	size_t	len = strlen(s);
	size_t	i = 0;

	while (i < len) {
		if (!is_digit(s[i]))
			return (0);
	}
	return (1);
}
