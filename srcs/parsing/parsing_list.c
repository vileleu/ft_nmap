/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:13:36 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/27 14:37:23 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_list      *atoi_list(t_get_arg *tmp, const char *s) {
	size_t		i = 0;
	uint16_t	*data;
	uint32_t	nb = 0;

	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
		i++;
	while (s[i] || s[i] == SEPARATOR_STR || isdigit(s[i])) {
		while (s[i] == SEPARATOR_STR)
			i++;
		if (isdigit(s[i])) {
			while (isdigit(s[i]))
				nb = nb * 10 + (s[i++] - '0');
            if (same_list_nb(tmp->un.list, nb) || nb > MAX_PORT) {
                (nb > MAX_PORT) ? (tmp->error = ERROR_LIMIT) : (tmp->error = ERROR_SAME);
                return (NULL);
            }
			if (!(data = malloc(sizeof(uint16_t))))
				return (NULL);
			*data = nb;
			if (add_list(&tmp->un.list, data))
				return (NULL);
			nb = 0;
		}
	}
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
		i++;
	if (s[i])
		return (NULL);
	return (tmp->un.list);
}

t_list_addr	*add_list_addr(t_list_addr **list, char *data) {
	t_list_addr	*new = NULL;
	t_list_addr	*tmp = *list;
	
	if (!(new = malloc(sizeof(t_list_addr)))) {
		free(data);
		return (NULL);
	}
	new->data = data;
	new->next = NULL;
	if (!*list) {
		*list = new;
		return (new);
	}
	else {
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (new);
}

uint8_t		add_list(t_list **list, uint16_t *data) {
	t_list	*new = NULL;
	t_list	*tmp = *list;
    t_list	*save = *list;
	
	if (!(new = malloc(sizeof(t_list)))) {
		free(data);
		return (EXIT_FAILURE);
	}
	new->data = data;
	new->next = NULL;
	if (!*list) {
		*list = new;
		return (EXIT_SUCCESS);
	}
	if (*data < *tmp->data) {
		*list = new;
		new->next = tmp;
		return (EXIT_SUCCESS);
	}
    while (tmp && *data > *tmp->data) {
        save = tmp;
	    tmp = tmp->next;
    }
    save->next = new;
	new->next = tmp;
	return (EXIT_SUCCESS);
}

uint8_t		str_islist_nb(const char *s) {
	size_t	i = 0;
	uint8_t	sep = 0;
	uint8_t	check = 0;

	if (!s || !s[i])
		return (0);
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
		i++;
	while (isdigit(s[i]) || s[i] == SEPARATOR_STR) {
		if (s[i] == SEPARATOR_STR)
			sep = 1;
		if (isdigit(s[i]))
			check = 1;
		i++;
	}
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
		i++;
	if (s[i] || !check || !sep)
		return (0);
	return (1);
}

uint8_t     same_list_addr(t_list_addr *list, char *host) {
    while (list) {
        if (!strcmp(list->data, host))
            return (1);
        list = list->next;
    }
    return (0);
}

uint8_t     same_list_nb(t_list *list, uint16_t nb) {
    while (list) {
        if (*(uint16_t *)list->data == nb)
            return (1);
        list = list->next;
    }
    return (0);
}

t_list_addr	*last_list_addr(t_list_addr *list) {
	while (list->next)
		list = list->next;
	return (list);
}

t_list		*last_list(t_list *list) {
	while (list->next)
		list = list->next;
	return (list);
}

uint32_t	count_list(t_list *list) {
	uint32_t	count = 0;

	while (list->next) {
		count++;
		list = list->next;
	}
	return (count);
}
