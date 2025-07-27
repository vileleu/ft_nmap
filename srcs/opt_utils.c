/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 23:07:01 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/27 17:13:01 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

void	free_list(t_list *list) {
	t_list	*tmp = NULL;

	while (list) {
		tmp = list;
		list = list->next;
		free(tmp->data);
		free(tmp);
	}
}

uint8_t		add_list(t_list **list, void *data) {
	t_list	*new = NULL;
	t_list	*tmp = *list;
	
	if (!(new = malloc(sizeof(t_list))))
		return (EXIT_FAILURE);
	new->data = data;
	new->next = NULL;
	if (!*list) {
		*list = new;
		return (EXIT_SUCCESS);
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (EXIT_SUCCESS);
}

t_list		*last_list(t_list *list) {
	while (list->next)
		list = list->next;
	return (list);
}

void    free_opt(t_opt *opt) {
    free_list(opt->targets);
    free(opt);
}

void	print_opt(const char *name, t_opt *opt) {
	t_list	*tmp = opt->targets;
	uint8_t	i = 0;

	printf("%s:\n", name);
	// port
	(opt->port.isranged ? printf("port: %d-%d\n", opt->port.min, opt->port.max) : printf("port: %d\n", opt->port.port));
	// ip/host
	printf("targets: ");
	while (tmp) {
		printf("|%s|", (char *)tmp->data);
		tmp = tmp->next;
		if (tmp)
			printf(" ");
	}
	printf("\n");
	// thread
	printf("thread: %d\n", opt->thread);
	printf("scan: ");
	while (i < 6) {
		printf("|%d|", opt->scan[i++]);
		if (i < 6)
			printf(" ");
	}
	printf("\n");
}