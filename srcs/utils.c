/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 23:07:01 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/25 23:08:20 by vileleu          ###   ########.fr       */
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

void    free_opt(t_opt *opt) {
    free_list(opt->targets);
    free(opt);
}

void	print_opt(const char *name, t_opt *opt) {
	t_list	*tmp = opt->targets;
	printf("%s:\n", name);
	// port
	(opt->port.isranged ? printf("port: %d-%d\n", opt->port.min, opt->port.max) : printf("port: %d\n", opt->port.port));
	// ip/host
	printf("targets: ");
	while (tmp) {
		printf("%s ", (char *)tmp->data);
		tmp = tmp->next;
	}
	printf("\n");
	// thread
	printf("thread: %d\n", opt->thread);
}