/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:44:56 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/28 22:56:32 by vileleu          ###   ########.fr       */
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
	if (isdigit(s[i]) || isdigit(s[i + 1]))
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

char	*ft_strjoin(char *s1, char const *s2) {
	char		*str;
	uint32_t	len_s1 = 0;
	uint32_t	len_s2 = 0;

	if (s2 == NULL)
		return (NULL);
	if (s1)
		len_s1 = strlen(s1);
	len_s2 = strlen(s2);
	if (s2[len_s2 - 1] == '\n')
		len_s2--;
	if (!(str = malloc(sizeof(char) * (len_s1 + len_s2 + 1))))
		return (NULL);
	memmove(str, s1, len_s1);
	memmove(str + len_s1, s2, len_s2);
	str[len_s1 + len_s2] = '\0';
	if (s1)
		free(s1);
	return (str);
}

uint8_t		str_ishost(t_parse *parse, t_list_addr *list, char *host) {
	struct addrinfo	hints, *res;
	int				ret;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	if ((ret = getaddrinfo(host, NULL, &hints, &res)))
		return error_parsing_example(parse, host, gai_strerror(ret), parse->i + parse->skip_next);
	else {
		list->addr = *(struct sockaddr_in *)res->ai_addr;
		freeaddrinfo(res);
	}
	return (EXIT_SUCCESS);
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
